MQTT_Subscribe_BoundedTopicFilter proof
=======================================

This directory contains a memory safety proof for `MQTT_Subscribe` along the
validation path that exercises the file-local `checkWildcardSubscriptions`
helper.

`checkWildcardSubscriptions` scans the caller-supplied topic filter for the
two wildcard characters defined by the MQTT specification. The scan must be
bounded by the caller-supplied `MQTTSubscribeInfo_t::topicFilterLength`
because `pTopicFilter` is a length-prefixed buffer that is not required to
be NUL-terminated.

The proof confirms that, when invoked along the wildcard-validation path,
`MQTT_Subscribe` never reads past the end of the `pTopicFilter` buffer that
`allocateMqttSubscriptionList()` allocated for it.

To run the proof.
* Add cbmc, goto-cc, goto-instrument, goto-analyzer, and cbmc-viewer
  to your path.
* Run "make".
* Open html/index.html in a web browser.
