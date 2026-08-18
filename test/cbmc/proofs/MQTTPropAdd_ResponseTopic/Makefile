#

MAX_RESPONSE_TOPIC_LENGTH=32

HARNESS_ENTRY=harness
HARNESS_FILE=MQTTPropAdd_ResponseTopic_harness
PROOF_UID=MQTTPropAdd_ResponseTopic

DEFINES += -DMAX_RESPONSE_TOPIC_LENGTH=$(MAX_RESPONSE_TOPIC_LENGTH)
INCLUDES +=

REMOVE_FUNCTION_BODY +=

UNWINDSET +=

PROOF_SOURCES += $(PROOFDIR)/$(HARNESS_FILE).c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/sources/mqtt_cbmc_state.c
# MQTTPropAdd_ResponseTopic uses memchr() to reject wildcard characters in the
# topic. Link the memchr stub so the call is modeled by the proof.
PROOF_SOURCES += $(SRCDIR)/test/cbmc/stubs/memchr.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_serializer.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_serializer_private.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_prop_serializer.c

include ../Makefile.common
