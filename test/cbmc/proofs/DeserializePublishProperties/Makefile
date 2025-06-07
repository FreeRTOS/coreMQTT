#
# Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

MQTT_INCOMING_PUBLISH_MAX_PROPERTIES=14
MAX_UTF_8_STRING_LENGTH=10

HARNESS_ENTRY=harness
HARNESS_FILE=DeserializePublishProperties_harness
PROOF_UID=DeserializePublishProperties

DEFINES += -DMAX_UTF_8_STRING_LENGTH=$(MAX_UTF_8_STRING_LENGTH)
INCLUDES +=

# These functions have their memory saftey proven in other harnesses.
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscard
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscardutf_8
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscard_uint8
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscard_uint32
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_mqtt_serializer_c_decodeuint32_t
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_mqtt_serializer_c_decodeuint16_t
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_mqtt_serializer_c_decodeuint8_t
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_mqtt_serializer_c_decodeutf_8
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_mqtt_serializer_c_decodeBinaryData

UNWINDSET += encodeVariableLength.0:5
UNWINDSET += __CPROVER_file_local_core_mqtt_serializer_c_decodeVariableLength.0:5
UNWINDSET += __CPROVER_file_local_core_mqtt_serializer_c_deserializePublishProperties.0:$(MQTT_INCOMING_PUBLISH_MAX_PROPERTIES)

PROOF_SOURCES += $(PROOFDIR)/$(HARNESS_FILE).c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/sources/mqtt_cbmc_state.c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/stubs/deserialize_helpers_stubs.c
PROOF_SOURCES += $(SRCDIR)/source/core_mqtt_utils.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_serializer.c

include ../Makefile.common
