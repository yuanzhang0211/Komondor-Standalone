#!/usr/bin/env python3
"""Read one AP's own JSONL stream; no global state assembly or peer file access."""
import argparse
import json
from pathlib import Path


def iter_ap_states(directory, ap_id):
    """Yield complete records from this AP only; ignore an unfinished trailing write."""
    if not isinstance(ap_id, int) or ap_id < 0:
        raise ValueError('ap_id must be a non-negative native node ID')
    previous_time = -1.0
    sequence = 0
    with (Path(directory)/f'ap_{ap_id}.jsonl').open() as stream:
        for line in stream:
            if not line.endswith('\n'):
                break
            state = json.loads(line)
            if state['schema'] != 'komondor.ap_state.v1' or state['ap_id'] != ap_id:
                raise ValueError('AP state identity/schema mismatch')
            if state['sequence'] != sequence or state['time_s'] < previous_time:
                raise ValueError('AP state sequence/time is not monotonic')
            sequence += 1
            previous_time = state['time_s']
            yield state


def latest_ap_state(directory, ap_id):
    latest = None
    for state in iter_ap_states(directory, ap_id):
        latest = state
    return latest


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('directory', type=Path)
    p.add_argument('--ap', required=True, type=int)
    p.add_argument('--all-events', action='store_true', help='Print the history of this AP only')
    args = p.parse_args()
    if args.all_events:
        for state in iter_ap_states(args.directory, args.ap):
            print(json.dumps(state, ensure_ascii=False))
    else:
        print(json.dumps(latest_ap_state(args.directory, args.ap), ensure_ascii=False, indent=2))


if __name__ == '__main__':
    main()
