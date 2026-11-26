# Spike 0003: Bundle transfer from Android

- Status: Planned
- Needs the pin: no (a second phone acting as a peripheral)
- Time box: one day (suggested)
- Feeds: [protocol.md](../protocol.md)

## Question

How long does it take an Android phone to send one bundle (about 4 kB in about 20 writes)
with Write Without Response, and how often does a write fail to arrive?

## Why it matters

The protocol uses Write Without Response for the images and relies on a CRC-32 and a resend
request for safety. The header layout and the acknowledgement format should be frozen
knowing how the transfer really behaves.

## Known before starting

- Android sends one GATT operation at a time. The app waits for the write callback before
  the next write, including for Write Without Response. Sending "as fast as possible" is not
  a real mode.
- The MTU is whatever the two sides negotiate. The app uses the value it is given.

## Method

1. Set up a GATT server on a second phone, with the proposed characteristics: nRF Connect, or
   a small test app if nRF Connect cannot log what is needed.
2. From a test app, send a bundle, waiting for each write callback.
3. Record the negotiated MTU, the total time and, from the server's log, whether every write
   arrived and in what order.
4. Repeat with and without `CONNECTION_PRIORITY_HIGH`, and at a few metres of distance.

Limit: nRF Connect cannot check a CRC or answer on Control. Loss is read from its log.

## Done when

- The transfer time and the loss rate are known for each setting.
- It is clear whether resending the whole bundle on error is acceptable, or whether a
  partial resend is worth its complexity.

## Result

Not run.

## Outcome

To be written. Expected: the open questions on framing closed in protocol.md.
