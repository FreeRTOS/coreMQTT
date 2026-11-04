# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT-0

HARNESS_ENTRY=harness
HARNESS_FILE=MQTT_ReceiveLoop_harness
PROOF_UID=MQTT_ReceiveLoop

# Bound on the timeout in MQTT_ProcessLoop. This timeout is bounded because
# memory saftey can be proven in a only a few iteration of the MQTT operations.
# Each iteration will try to receive a single packet in its entirety. With a time
# out of 2 we can get coverage of the entire function. Another iteration will
# performed unnecessarily duplicating of the proof.
MQTT_RECEIVE_TIMEOUT=3
# The NetworkInterfaceReceiveStub is called once for getting the incoming packet
# type with one byte of data, then it is called multiple times to reveive the
# packet.
MAX_NETWORK_RECV_TRIES=4
# Please see test/cbmc/stubs/network_interface_subs.c for
# more information on MAX_NETWORK_SEND_TRIES.
MAX_NETWORK_SEND_TRIES=3
# Please see test/cbmc/include/core_mqtt_config.h for more
# information.
MQTT_STATE_ARRAY_MAX_COUNT=11

# Maximum number of vecotrs to be sent in the call to process loop.
MAX_SEND_VECTORS=10

DEFINES += -DMQTT_RECEIVE_TIMEOUT=$(MQTT_RECEIVE_TIMEOUT)
DEFINES += -DMAX_NETWORK_SEND_TRIES=$(MAX_NETWORK_SEND_TRIES)
DEFINES += -DMAX_NETWORK_RECV_TRIES=$(MAX_NETWORK_RECV_TRIES)
INCLUDES +=

# These functions have their memory saftey proven in other harnesses.
REMOVE_FUNCTION_BODY += MQTT_DeserializeAck
REMOVE_FUNCTION_BODY += MQTT_SerializeAck
REMOVE_FUNCTION_BODY += MQTT_SerializeDisconnect
REMOVE_FUNCTION_BODY += MQTT_DeserializePublish
REMOVE_FUNCTION_BODY += MQTT_DeserializeDisconnect
REMOVE_FUNCTION_BODY += MQTT_ProcessIncomingPacketTypeAndLength # So that we can control when the loop exits.
REMOVE_FUNCTION_BODY += memmove # Use stub

# The loops below are unwound once more than the exclusive timeout bound.
UNWINDSET += __CPROVER_file_local_core_mqtt_c_discardStoredPacket.0:$(MAX_NETWORK_RECV_TRIES)
UNWINDSET += __CPROVER_file_local_core_mqtt_c_recvExact.0:$(MAX_NETWORK_RECV_TRIES)
# Unlike recvExact, sendBuffer is not bounded by the timeout. The loop in
# sendBuffer will continue until all the bytes are sent or a network error
# occurs. Please see NetworkInterfaceReceiveStub in
# libraries\standard\mqtt\cbmc\stubs\network_interface_stubs.c for more
# information.
UNWINDSET += __CPROVER_file_local_core_mqtt_c_sendBuffer.0:$(MAX_NETWORK_SEND_TRIES)
# These loops will run for the maximum number of publishes pending acknowledgement.
# This is set in test/cbmc/include/core_mqtt_config.h.
UNWINDSET += __CPROVER_file_local_core_mqtt_state_c_addRecord.0:$(MQTT_STATE_ARRAY_MAX_COUNT)
UNWINDSET += __CPROVER_file_local_core_mqtt_state_c_findInRecord.0:$(MQTT_STATE_ARRAY_MAX_COUNT)
# This loop will run only once.
UNWINDSET += __CPROVER_file_local_core_mqtt_c_receiveSingleIteration.0:5
UNWINDSET += __CPROVER_file_local_core_mqtt_c_sendMessageVector.0:${MAX_SEND_VECTORS}
UNWINDSET += __CPROVER_file_local_core_mqtt_c_sendMessageVector.1:${MAX_SEND_VECTORS}
UNWINDSET += __CPROVER_file_local_core_mqtt_c_sendMessageVector.2:${MAX_SEND_VECTORS}

PROOF_SOURCES += $(PROOFDIR)/$(HARNESS_FILE).c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/sources/mqtt_cbmc_state.c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/stubs/network_interface_stubs.c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/stubs/get_time_stub.c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/stubs/event_callback_stub.c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/stubs/memmove.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_serializer.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_serializer_private.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_state.c

EXPENSIVE = true

include ../Makefile.common
