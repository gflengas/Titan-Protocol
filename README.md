# Titan Protocol Encode / Decode

We would like you to spend approximately one hour on this task. We are not looking for a robust implementation, or even a complete implementation.

We are not expecting to see a full project with a Makefile/CMakelists/... build system and CI, a long gcc command is fine. We just want a piece of code we can all understand and talk about.

As a final note, please do not use an LLM to complete the assignment. We want to have a discussion about your code, not somebody else's code.

## Problem Statement

Your task is to write a parser for the _Titan Protocol_.

Your solution should be written in C or C++.

Several samples files are attached. Each line of the files is a new message.

## The Titan Protocol

A hypothetical Space company, Titan Space, uses a serial protocol (the _Titan Protocol_) to communicate with their sun-sensors.

> A sun sensor is a device which measures the coarse direction to the sun.

Each _Titan Protocol_ packet has the following structure:

- Start of Frame marker '$'
- Command Id
- Sequence Number
- Timestamp (number of milliseconds elapsed)
- Payload
- End of Frame marker `|`

Each field between the start of frame marker `$` and the end of frame marker `|` (including the fields of the payload), is separated by `;`.

For example:

```text
$10;16;4992;0;29880;-81173;74;-29402;|
```

This message has a command Id of `10`, a sequence number of `16`, and a timestamp of `4992` (this packet was sent after 4.992 s of operation). The sequence \[`0`, `29880`, `-81173`, `74`, `-29402`\] is the payload.

### Command Ids

In the capture you are given, there are two types of messages:

| Decimal Id | Description   |
| ---------- | ------------- |
| `001`      | Ping          |
| `012`      | House Keeping |

#### Ping Payload

The `Ping` packet has an empty payload.

### HouseKeeping Payload

The housekeeping packet has the following payload

| Name        | Description                                                      |
| ----------- | ---------------------------------------------------------------- |
| Status      | `0` indicates operational state, other values indicate failures  |
| Theta       | Theta angle of the sun position in increments of 1 / 1000 degree |
| Phi         | Phi angle of the sun position in increments of 1 / 1000 degrees  |
| Intensity   | Unitless quantity, Intensity of the sun on the sensor            |
| Temperature | Temperature of the sensor, increments of 1 / 1000 degree         |
