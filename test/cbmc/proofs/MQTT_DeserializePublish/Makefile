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

# This is the maximum number of properties that can fit in an incoming
# publish packet while keeping the constraints of the MQTT specification.
# MQTT_MAX_REMAINING_LENGTH=268435455
# M = Maximum bytes allowed for storing properties	 =	  MQTT_MAX_REMAINING_LENGTH
#												  		- Minimum topic name size ( 3, one character topic name )
#												  		- Minimum packet ID size ( 0, for QoS0 )
#												  		- Property length size ( 4 )
#												  		- Minimum payload size ( 0, no payload )
#
# In order to fit the maximum number of properties in M bytes, we will try to fit as
# many user properties as posible along with an optimal number of non-user properties.
# The optimal value is calculated to be: 
# Minimum size of a user property									= 7
# Optimal Number of user properties 								= ( M/7 ) - 3
# Space remaining after adding the user properties					= ( 7 * 3 ) + ( M mod 7) 
#																	= 22
# Maximum number of non-user propeties that can fit in 22 bytes     = 6
# ( Payload Format, Message Expiry,  Topic Alias, 
#   Response Topic, Co-relation Data, Content Type )
# Maximum number of properties that can fit in publish packet		= ( M/7 ) - 3 + 6
#																	= ( M/7 ) + 3
#																	= 38347925
# Hence the value of the following macro will be 1 greater than this value, as it is used for loop unwindset.
# MQTT_INCOMING_PUBLISH_MAX_PROPERTIES=38347926
MQTT_INCOMING_PUBLISH_MAX_PROPERTIES=2

HARNESS_ENTRY=harness
HARNESS_FILE=MQTT_DeserializePublish_harness
PROOF_UID=MQTT_DeserializePublish

DEFINES +=
INCLUDES +=

REMOVE_FUNCTION_BODY +=
UNWINDSET += __CPROVER_file_local_core_mqtt_serializer_c_decodeVariableLength.0:5
UNWINDSET += __CPROVER_file_local_core_mqtt_serializer_c_deserializePublishProperties.0:$(MQTT_INCOMING_PUBLISH_MAX_PROPERTIES)

PROOF_SOURCES += $(PROOFDIR)/$(HARNESS_FILE).c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/sources/mqtt_cbmc_state.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_serializer.c

include ../Makefile.common
