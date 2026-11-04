#

MAX_CONTENT_TYPE_LENGTH=32

HARNESS_ENTRY=harness
HARNESS_FILE=MQTTPropAdd_ContentType_harness
PROOF_UID=MQTTPropAdd_ContentType

DEFINES += -DMAX_CONTENT_TYPE_LENGTH=$(MAX_CONTENT_TYPE_LENGTH)
INCLUDES +=

REMOVE_FUNCTION_BODY +=

UNWINDSET +=

PROOF_SOURCES += $(PROOFDIR)/$(HARNESS_FILE).c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/sources/mqtt_cbmc_state.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_serializer.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_serializer_private.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt_prop_serializer.c

include ../Makefile.common
