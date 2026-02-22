import sqlite3
import time
import os

db_path= os.getenv('DB_PATH', "dns.db")


def get_connection() -> sqlite3.Connection:

    conn = sqlite3.connect(db_path)
    conn.row_factory = sqlite3.Row
    return conn


def init_db() -> None:
    print("Initializing DB")
    with get_connection() as conn:
        conn.executescript("""
            BEGIN;
            CREATE TABLE IF NOT EXISTS records (
                id         INTEGER PRIMARY KEY AUTOINCREMENT,
                name       TEXT NOT NULL,
                type       TEXT NOT NULL,
                value      TEXT NOT NULL,
                ttl        INTEGER NOT NULL,
                expires_at INTEGER
            );
            CREATE INDEX IF NOT EXISTS idx_name_type ON records (name, type);
            COMMIT;
        """)


def insert_record(name: str, record_type: str, value: str, ttl: int) -> None:
    expires_at = int(time.time()) + ttl

    existent_record = get_record(name, record_type)

    # for now only one record for one type
    if existent_record:
        delete_record(name, record_type)

    with get_connection() as conn:
        conn.execute(
            "INSERT INTO records (name, type, value, ttl, expires_at) VALUES (?, ?, ?, ?, ?)",
            (name, record_type, value, ttl, expires_at),
        )


def get_record(name: str, record_type: str) -> list[sqlite3.Row]:
    purge_expired()
    now = int(time.time())
    with get_connection() as conn:
        return conn.execute(
            """
            SELECT *, (expires_at - ?) AS remaining_ttl
            FROM records
            WHERE name = ? AND type = ? AND (expires_at IS NULL OR expires_at > ?)
            """,
            (now, name, record_type, now),
        ).fetchone()


def delete_record(name: str, record_type: str) -> bool:
    with get_connection() as conn:
        cur = conn.execute(
            "DELETE FROM records WHERE name = ? AND type = ?",
            (name, record_type),
        )
        return cur.rowcount > 0

def purge_expired() -> int:
    with get_connection() as conn:
        cur = conn.execute(
            "DELETE FROM records WHERE expires_at IS NOT NULL AND expires_at <= ?",
            (int(time.time()),)
        )
        return cur.rowcount
