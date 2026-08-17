MQTTPropAdd_CorrelationData proof
=================================

This directory contains a memory safety proof for MQTTPropAdd_CorrelationData.

MQTTPropAdd_CorrelationData is a public entry point into the file-local addPropUtf8
helper, which serializes a UTF-8 string property into the property builder
buffer as:

    [ 1 byte property ID ][ 2 byte UTF-8 length prefix ][ propertyLength bytes ]

The proof confirms that the bounds check in addPropUtf8 reserves space for all
3 + propertyLength bytes (including the property-ID byte) and therefore never
writes past the end of the property builder buffer.

To run the proof.
* Add cbmc, goto-cc, goto-instrument, goto-analyzer, and cbmc-viewer
  to your path.
* Run "make".
* Open html/index.html in a web browser.
