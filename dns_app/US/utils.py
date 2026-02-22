
def parse_dns_record(record):
    parsed = {}

    for token in record.replace('\n', ' ').split(' '):
        if '=' in token:
            key, _, value = token.partition('=')
            parsed[key.strip()] = value.strip()

    return parsed
