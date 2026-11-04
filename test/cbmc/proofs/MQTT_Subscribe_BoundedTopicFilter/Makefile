#
# Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
#
# SPDX-License-Identifier: MIT
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

HARNESS_ENTRY=harness
HARNESS_FILE=MQTT_Subscribe_BoundedTopicFilter_harness
PROOF_UID=MQTT_Subscribe_BoundedTopicFilter

# Bound on the topic-filter length used by the proof. Kept small so the proof
# terminates quickly. The OOB-read property does not depend on the length, so
# a tight bound is sufficient to expose any over-read.
MAX_TOPIC_NAME_FILTER_LENGTH=4
# Loops that iterate over the subscription list (allocateMqttSubscriptionList,
# validateSubscribeUnsubscribeParams, calculateSubscriptionPacketSize) need an
# unwind bound of count+1 to discharge their termination assertions. The harness
# uses a single subscription, so this must be at least 2 (matches the upstream
# MQTT_Subscribe proof).
SUBSCRIPTION_COUNT_MAX=2
SUBSCRIBE_PACKET_VECTORS=7
MAX_NETWORK_SEND_TRIES=3

DEFINES += -DMAX_TOPIC_NAME_FILTER_LENGTH=$(MAX_TOPIC_NAME_FILTER_LENGTH)
DEFINES += -DSUBSCRIPTION_COUNT_MAX=$(SUBSCRIPTION_COUNT_MAX)
DEFINES += -DMAX_NETWORK_SEND_TRIES=$(MAX_NETWORK_SEND_TRIES)
INCLUDES +=

REMOVE_FUNCTION_BODY += MQTT_ValidateSubscribeProperties

# Bound the scan-for-wildcard loops by the topic filter length. The harness
# allocates pTopicFilter to exactly topicFilterLength bytes, so any iteration
# past that bound is an out-of-bounds access CBMC will report.
UNWINDSET += strchr.0:$(MAX_TOPIC_NAME_FILTER_LENGTH)
UNWINDSET += memchr.0:$(MAX_TOPIC_NAME_FILTER_LENGTH)
UNWINDSET += allocateMqttSubscriptionList.0:$(SUBSCRIPTION_COUNT_MAX)
UNWINDSET += __CPROVER_file_local_core_mqtt_c_validateSharedSubscriptions.0:$(MAX_TOPIC_NAME_FILTER_LENGTH)
UNWINDSET += __CPROVER_file_local_core_mqtt_serializer_c_calculateSubscriptionPacketSize.0:$(SUBSCRIPTION_COUNT_MAX)
UNWINDSET += __CPROVER_file_local_core_mqtt_c_validateSubscribeUnsubscribeParams.0:$(SUBSCRIPTION_COUNT_MAX)
UNWINDSET += __CPROVER_file_local_core_mqtt_c_validateSubscribeUnsubscribeParams.1:$(SUBSCRIPTION_COUNT_MAX)
UNWINDSET += __CPROVER_file_local_core_mqtt_c_sendMessageVector.0:${SUBSCRIBE_PACKET_VECTORS}
UNWINDSET += __CPROVER_file_local_core_mqtt_c_sendMessageVector.1:${SUBSCRIBE_PACKET_VECTORS}
UNWINDSET += __CPROVER_file_local_core_mqtt_c_sendMessageVector.2:${SUBSCRIBE_PACKET_VECTORS}
UNWINDSET += encodeVariableLength.0:5
UNWINDSET += __CPROVER_file_local_core_mqtt_c_sendSubscribeWithoutCopy.0:$(MAX_NETWORK_SEND_TRIES)
UNWINDSET += __CPROVER_file_local_core_mqtt_c_sendSubscribeWithoutCopy.1:$(MAX_NETWORK_SEND_TRIES)

PROOF_SOURCES += $(PROOFDIR)/$(HARNESS_FILE).c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/sources/mqtt_cbmc_state.c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/stubs/network_interface_stubs.c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/stubs/get_time_stub.c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/stubs/event_callback_stub.c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/stubs/memchr.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_serializer.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_serializer_private.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_state.c

include ../Makefile.common
