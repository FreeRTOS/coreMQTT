MQTT_ReserveState proof
==============

This directory contains a memory safety proof for MQTT_ReserveState.

NOTE: This proof is EXPECTED TO FAIL. It exposes the genuine out-of-bounds read
in addRecord (finding state01_01), reached via MQTT_ReserveState on a context
whose outgoing publish record array is NULL / max count 0.

To run the proof.
* Add cbmc, goto-cc, goto-instrument, goto-analyzer, and cbmc-viewer
  to your path.
* Run "make".
* Open html/index.html in a web browser.
