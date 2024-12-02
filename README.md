Raft
======
A mimic Raft implementation of the cornerstone project, which is a very lightweight C++ implementation with minimum dependencies, originally written by [Andy Chen](https://github.com/andy-yx-chen).

## Features

- [x] Core Raft algorithm
    * Log replication & compaction
    * Leader election
    * Snapshot
    * Dynamic membership & configuration change
- [x] Configuration change support, add or remove servers without any limitation
- [x] Log compaction
- [x] **Urgent commit**, enables the leader to ask all other peers to commit one or more logs if commit index is advanced
- [x] Client request support, for each server, the state machine could get a raft consensus client to send request to the leader, so leader change listener is not required.
## Build and run

```bash
git clone https://github.com/iTomGeller/Raft.git
cd Raft
git submodule update --init --recursive
mkdir build && cd build
cmake ..
make
./test
```
