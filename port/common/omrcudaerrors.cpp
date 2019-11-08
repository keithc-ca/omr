#include <cuda.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdint.h>
#include "omrcuda.h"

struct Mapping
{
	const char *omrName;
	J9CudaError omrCode;
	const char *cudaName;
	cudaError_t cudaCode;
};

#define MakeMapping(omrCode, cudaCode) \
	{ #omrCode, omrCode, #cudaCode, cudaCode }

static const Mapping mappings[] =
{
	MakeMapping(J9CUDA_NO_ERROR, cudaSuccess),
	MakeMapping(J9CUDA_ERROR_MISSING_CONFIGURATION, cudaErrorMissingConfiguration),
	MakeMapping(J9CUDA_ERROR_MEMORY_ALLOCATION, cudaErrorMemoryAllocation),
	MakeMapping(J9CUDA_ERROR_INITIALIZATION_ERROR, cudaErrorInitializationError),
	MakeMapping(J9CUDA_ERROR_LAUNCH_FAILURE, cudaErrorLaunchFailure),
	MakeMapping(J9CUDA_ERROR_LAUNCH_TIMEOUT, cudaErrorLaunchTimeout),
	MakeMapping(J9CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES, cudaErrorLaunchOutOfResources),
	MakeMapping(J9CUDA_ERROR_INVALID_DEVICE_FUNCTION, cudaErrorInvalidDeviceFunction),
	MakeMapping(J9CUDA_ERROR_INVALID_CONFIGURATION, cudaErrorInvalidConfiguration),
	MakeMapping(J9CUDA_ERROR_INVALID_DEVICE, cudaErrorInvalidDevice),
	MakeMapping(J9CUDA_ERROR_INVALID_VALUE, cudaErrorInvalidValue),
	MakeMapping(J9CUDA_ERROR_INVALID_PITCH_VALUE, cudaErrorInvalidPitchValue),
	MakeMapping(J9CUDA_ERROR_INVALID_SYMBOL, cudaErrorInvalidSymbol),
	MakeMapping(J9CUDA_ERROR_MAP_BUFFER_OBJECT_FAILED, cudaErrorMapBufferObjectFailed),
	MakeMapping(J9CUDA_ERROR_UNMAP_BUFFER_OBJECT_FAILED, cudaErrorUnmapBufferObjectFailed),
	MakeMapping(J9CUDA_ERROR_INVALID_HOST_POINTER, cudaErrorInvalidHostPointer),
	MakeMapping(J9CUDA_ERROR_INVALID_DEVICE_POINTER, cudaErrorInvalidDevicePointer),
	MakeMapping(J9CUDA_ERROR_INVALID_TEXTURE, cudaErrorInvalidTexture),
	MakeMapping(J9CUDA_ERROR_INVALID_TEXTURE_BINDING, cudaErrorInvalidTextureBinding),
	MakeMapping(J9CUDA_ERROR_INVALID_CHANNEL_DESCRIPTOR, cudaErrorInvalidChannelDescriptor),
	MakeMapping(J9CUDA_ERROR_INVALID_MEMCPY_DIRECTION, cudaErrorInvalidMemcpyDirection),
	MakeMapping(J9CUDA_ERROR_INVALID_FILTER_SETTING, cudaErrorInvalidFilterSetting),
	MakeMapping(J9CUDA_ERROR_INVALID_NORM_SETTING, cudaErrorInvalidNormSetting),
	MakeMapping(J9CUDA_ERROR_CUDART_UNLOADING, cudaErrorCudartUnloading),
	MakeMapping(J9CUDA_ERROR_UNKNOWN, cudaErrorUnknown),
	MakeMapping(J9CUDA_ERROR_INVALID_RESOURCE_HANDLE, cudaErrorInvalidResourceHandle),
	MakeMapping(J9CUDA_ERROR_NOT_READY, cudaErrorNotReady),
	MakeMapping(J9CUDA_ERROR_INSUFFICIENT_DRIVER, cudaErrorInsufficientDriver),
	MakeMapping(J9CUDA_ERROR_SET_ON_ACTIVE_PROCESS, cudaErrorSetOnActiveProcess),
	MakeMapping(J9CUDA_ERROR_INVALID_SURFACE, cudaErrorInvalidSurface),
	MakeMapping(J9CUDA_ERROR_NO_DEVICE, cudaErrorNoDevice),
	MakeMapping(J9CUDA_ERROR_ECCUNCORRECTABLE, cudaErrorECCUncorrectable),
	MakeMapping(J9CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND, cudaErrorSharedObjectSymbolNotFound),
	MakeMapping(J9CUDA_ERROR_SHARED_OBJECT_INIT_FAILED, cudaErrorSharedObjectInitFailed),
	MakeMapping(J9CUDA_ERROR_UNSUPPORTED_LIMIT, cudaErrorUnsupportedLimit),
	MakeMapping(J9CUDA_ERROR_DUPLICATE_VARIABLE_NAME, cudaErrorDuplicateVariableName),
	MakeMapping(J9CUDA_ERROR_DUPLICATE_TEXTURE_NAME, cudaErrorDuplicateTextureName),
	MakeMapping(J9CUDA_ERROR_DUPLICATE_SURFACE_NAME, cudaErrorDuplicateSurfaceName),
	MakeMapping(J9CUDA_ERROR_DEVICES_UNAVAILABLE, cudaErrorDevicesUnavailable),
	MakeMapping(J9CUDA_ERROR_INVALID_KERNEL_IMAGE, cudaErrorInvalidKernelImage),
	MakeMapping(J9CUDA_ERROR_NO_KERNEL_IMAGE_FOR_DEVICE, cudaErrorNoKernelImageForDevice),
	MakeMapping(J9CUDA_ERROR_INCOMPATIBLE_DRIVER_CONTEXT, cudaErrorIncompatibleDriverContext),
	MakeMapping(J9CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED, cudaErrorPeerAccessAlreadyEnabled),
	MakeMapping(J9CUDA_ERROR_PEER_ACCESS_NOT_ENABLED, cudaErrorPeerAccessNotEnabled),
	MakeMapping(J9CUDA_ERROR_DEVICE_ALREADY_IN_USE, cudaErrorDeviceAlreadyInUse),
	MakeMapping(J9CUDA_ERROR_PROFILER_DISABLED, cudaErrorProfilerDisabled),
	MakeMapping(J9CUDA_ERROR_ASSERT, cudaErrorAssert),
	MakeMapping(J9CUDA_ERROR_TOO_MANY_PEERS, cudaErrorTooManyPeers),
	MakeMapping(J9CUDA_ERROR_HOST_MEMORY_ALREADY_REGISTERED, cudaErrorHostMemoryAlreadyRegistered),
	MakeMapping(J9CUDA_ERROR_HOST_MEMORY_NOT_REGISTERED, cudaErrorHostMemoryNotRegistered),
	MakeMapping(J9CUDA_ERROR_OPERATING_SYSTEM, cudaErrorOperatingSystem),
	MakeMapping(J9CUDA_ERROR_PEER_ACCESS_UNSUPPORTED, cudaErrorPeerAccessUnsupported),
	MakeMapping(J9CUDA_ERROR_LAUNCH_MAX_DEPTH_EXCEEDED, cudaErrorLaunchMaxDepthExceeded),
	MakeMapping(J9CUDA_ERROR_LAUNCH_FILE_SCOPED_TEX, cudaErrorLaunchFileScopedTex),
	MakeMapping(J9CUDA_ERROR_LAUNCH_FILE_SCOPED_SURF, cudaErrorLaunchFileScopedSurf),
	MakeMapping(J9CUDA_ERROR_SYNC_DEPTH_EXCEEDED, cudaErrorSyncDepthExceeded),
	MakeMapping(J9CUDA_ERROR_LAUNCH_PENDING_COUNT_EXCEEDED, cudaErrorLaunchPendingCountExceeded),
	MakeMapping(J9CUDA_ERROR_NOT_PERMITTED, cudaErrorNotPermitted),
	MakeMapping(J9CUDA_ERROR_NOT_SUPPORTED, cudaErrorNotSupported),
	MakeMapping(J9CUDA_ERROR_STARTUP_FAILURE, cudaErrorStartupFailure),
	MakeMapping(J9CUDA_ERROR_NOT_FOUND, (cudaError_t)-(int32_t)CUDA_ERROR_NOT_FOUND)
};

#define LENGTH_OF(array) (sizeof(array) / sizeof((array)[0]))

static void
printMap()
{
	bool trivial = true;

	printf("J9CudaError\n");
	printf("futureMapRuntime(cudaError_t cudaCode)\n");
	printf("{\n");

	for (int32_t i = 0; i < LENGTH_OF(mappings); ++i) {
		const Mapping * mapping = &mappings[i];

		/* only include entries for unequal values */
		if ((int32_t)mapping->omrCode != (int32_t)mapping->cudaCode) {
			if (trivial) {
				printf("\tswitch ((int32_t)cudaCode) {\n");
				trivial = false;
			}
			printf("\tcase %d: /* %s */\n", mapping->cudaCode, mapping->cudaName);
			printf("\t\treturn %s;\n", mapping->omrName);
		}
	}

	if (!trivial) {
		printf("\tdefault:\n");
		printf("\t\tbreak;\n");
		printf("\t}\n");
	}

	printf("\treturn (J9CudaError)(int32_t)cudaCode;\n");
	printf("}\n");
}

static void
printUnmap()
{
	bool trivial = true;

	printf("cudaError_t\n");
	printf("futureUnmapRuntime(J9CudaError omrCode)\n");
	printf("{\n");

	for (int32_t i = 0; i < LENGTH_OF(mappings); ++i) {
		const Mapping * mapping = &mappings[i];

		/* only include entries for unequal values */
		if ((int32_t)mapping->omrCode != (int32_t)mapping->cudaCode) {
			if (trivial) {
				printf("\tswitch (omrCode) {\n");
				trivial = false;
			}
			printf("\tcase %s:\n", mapping->omrName);
			printf("\t\treturn (cudaError_t)%d; /* %s */\n", mapping->cudaCode, mapping->cudaName);
		}
	}

	if (!trivial) {
		printf("\tdefault:\n");
		printf("\t\tbreak;\n");
		printf("\t}\n");
	}

	printf("\treturn (cudaError_t)(int32_t)omrCode;\n");
	printf("}\n");
}

int
main(int argc, char ** argv)
{
	printMap();
	printf("\n");
	printUnmap();

	return 0;
}
