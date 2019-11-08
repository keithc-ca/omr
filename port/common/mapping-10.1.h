J9CudaError
futureMapRuntime(cudaError_t cudaCode)
{
	switch ((int32_t)cudaCode) {
	case 52: /* cudaErrorMissingConfiguration */
		return J9CUDA_ERROR_MISSING_CONFIGURATION;
	case 719: /* cudaErrorLaunchFailure */
		return J9CUDA_ERROR_LAUNCH_FAILURE;
	case 702: /* cudaErrorLaunchTimeout */
		return J9CUDA_ERROR_LAUNCH_TIMEOUT;
	case 701: /* cudaErrorLaunchOutOfResources */
		return J9CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES;
	case 98: /* cudaErrorInvalidDeviceFunction */
		return J9CUDA_ERROR_INVALID_DEVICE_FUNCTION;
	case 101: /* cudaErrorInvalidDevice */
		return J9CUDA_ERROR_INVALID_DEVICE;
	case 1: /* cudaErrorInvalidValue */
		return J9CUDA_ERROR_INVALID_VALUE;
	case 205: /* cudaErrorMapBufferObjectFailed */
		return J9CUDA_ERROR_MAP_BUFFER_OBJECT_FAILED;
	case 206: /* cudaErrorUnmapBufferObjectFailed */
		return J9CUDA_ERROR_UNMAP_BUFFER_OBJECT_FAILED;
	case 4: /* cudaErrorCudartUnloading */
		return J9CUDA_ERROR_CUDART_UNLOADING;
	case 999: /* cudaErrorUnknown */
		return J9CUDA_ERROR_UNKNOWN;
	case 400: /* cudaErrorInvalidResourceHandle */
		return J9CUDA_ERROR_INVALID_RESOURCE_HANDLE;
	case 600: /* cudaErrorNotReady */
		return J9CUDA_ERROR_NOT_READY;
	case 708: /* cudaErrorSetOnActiveProcess */
		return J9CUDA_ERROR_SET_ON_ACTIVE_PROCESS;
	case 100: /* cudaErrorNoDevice */
		return J9CUDA_ERROR_NO_DEVICE;
	case 214: /* cudaErrorECCUncorrectable */
		return J9CUDA_ERROR_ECCUNCORRECTABLE;
	case 302: /* cudaErrorSharedObjectSymbolNotFound */
		return J9CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND;
	case 303: /* cudaErrorSharedObjectInitFailed */
		return J9CUDA_ERROR_SHARED_OBJECT_INIT_FAILED;
	case 215: /* cudaErrorUnsupportedLimit */
		return J9CUDA_ERROR_UNSUPPORTED_LIMIT;
	case 200: /* cudaErrorInvalidKernelImage */
		return J9CUDA_ERROR_INVALID_KERNEL_IMAGE;
	case 209: /* cudaErrorNoKernelImageForDevice */
		return J9CUDA_ERROR_NO_KERNEL_IMAGE_FOR_DEVICE;
	case 704: /* cudaErrorPeerAccessAlreadyEnabled */
		return J9CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED;
	case 705: /* cudaErrorPeerAccessNotEnabled */
		return J9CUDA_ERROR_PEER_ACCESS_NOT_ENABLED;
	case 216: /* cudaErrorDeviceAlreadyInUse */
		return J9CUDA_ERROR_DEVICE_ALREADY_IN_USE;
	case 5: /* cudaErrorProfilerDisabled */
		return J9CUDA_ERROR_PROFILER_DISABLED;
	case 710: /* cudaErrorAssert */
		return J9CUDA_ERROR_ASSERT;
	case 711: /* cudaErrorTooManyPeers */
		return J9CUDA_ERROR_TOO_MANY_PEERS;
	case 712: /* cudaErrorHostMemoryAlreadyRegistered */
		return J9CUDA_ERROR_HOST_MEMORY_ALREADY_REGISTERED;
	case 713: /* cudaErrorHostMemoryNotRegistered */
		return J9CUDA_ERROR_HOST_MEMORY_NOT_REGISTERED;
	case 304: /* cudaErrorOperatingSystem */
		return J9CUDA_ERROR_OPERATING_SYSTEM;
	case 217: /* cudaErrorPeerAccessUnsupported */
		return J9CUDA_ERROR_PEER_ACCESS_UNSUPPORTED;
	case 800: /* cudaErrorNotPermitted */
		return J9CUDA_ERROR_NOT_PERMITTED;
	case 801: /* cudaErrorNotSupported */
		return J9CUDA_ERROR_NOT_SUPPORTED;
	default:
		break;
	}
	return (J9CudaError)(int32_t)cudaCode;
}

cudaError_t
futureUnmapRuntime(J9CudaError omrCode)
{
	switch (omrCode) {
	case J9CUDA_ERROR_MISSING_CONFIGURATION:
		return (cudaError_t)52; /* cudaErrorMissingConfiguration */
	case J9CUDA_ERROR_LAUNCH_FAILURE:
		return (cudaError_t)719; /* cudaErrorLaunchFailure */
	case J9CUDA_ERROR_LAUNCH_TIMEOUT:
		return (cudaError_t)702; /* cudaErrorLaunchTimeout */
	case J9CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES:
		return (cudaError_t)701; /* cudaErrorLaunchOutOfResources */
	case J9CUDA_ERROR_INVALID_DEVICE_FUNCTION:
		return (cudaError_t)98; /* cudaErrorInvalidDeviceFunction */
	case J9CUDA_ERROR_INVALID_DEVICE:
		return (cudaError_t)101; /* cudaErrorInvalidDevice */
	case J9CUDA_ERROR_INVALID_VALUE:
		return (cudaError_t)1; /* cudaErrorInvalidValue */
	case J9CUDA_ERROR_MAP_BUFFER_OBJECT_FAILED:
		return (cudaError_t)205; /* cudaErrorMapBufferObjectFailed */
	case J9CUDA_ERROR_UNMAP_BUFFER_OBJECT_FAILED:
		return (cudaError_t)206; /* cudaErrorUnmapBufferObjectFailed */
	case J9CUDA_ERROR_CUDART_UNLOADING:
		return (cudaError_t)4; /* cudaErrorCudartUnloading */
	case J9CUDA_ERROR_UNKNOWN:
		return (cudaError_t)999; /* cudaErrorUnknown */
	case J9CUDA_ERROR_INVALID_RESOURCE_HANDLE:
		return (cudaError_t)400; /* cudaErrorInvalidResourceHandle */
	case J9CUDA_ERROR_NOT_READY:
		return (cudaError_t)600; /* cudaErrorNotReady */
	case J9CUDA_ERROR_SET_ON_ACTIVE_PROCESS:
		return (cudaError_t)708; /* cudaErrorSetOnActiveProcess */
	case J9CUDA_ERROR_NO_DEVICE:
		return (cudaError_t)100; /* cudaErrorNoDevice */
	case J9CUDA_ERROR_ECCUNCORRECTABLE:
		return (cudaError_t)214; /* cudaErrorECCUncorrectable */
	case J9CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND:
		return (cudaError_t)302; /* cudaErrorSharedObjectSymbolNotFound */
	case J9CUDA_ERROR_SHARED_OBJECT_INIT_FAILED:
		return (cudaError_t)303; /* cudaErrorSharedObjectInitFailed */
	case J9CUDA_ERROR_UNSUPPORTED_LIMIT:
		return (cudaError_t)215; /* cudaErrorUnsupportedLimit */
	case J9CUDA_ERROR_INVALID_KERNEL_IMAGE:
		return (cudaError_t)200; /* cudaErrorInvalidKernelImage */
	case J9CUDA_ERROR_NO_KERNEL_IMAGE_FOR_DEVICE:
		return (cudaError_t)209; /* cudaErrorNoKernelImageForDevice */
	case J9CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED:
		return (cudaError_t)704; /* cudaErrorPeerAccessAlreadyEnabled */
	case J9CUDA_ERROR_PEER_ACCESS_NOT_ENABLED:
		return (cudaError_t)705; /* cudaErrorPeerAccessNotEnabled */
	case J9CUDA_ERROR_DEVICE_ALREADY_IN_USE:
		return (cudaError_t)216; /* cudaErrorDeviceAlreadyInUse */
	case J9CUDA_ERROR_PROFILER_DISABLED:
		return (cudaError_t)5; /* cudaErrorProfilerDisabled */
	case J9CUDA_ERROR_ASSERT:
		return (cudaError_t)710; /* cudaErrorAssert */
	case J9CUDA_ERROR_TOO_MANY_PEERS:
		return (cudaError_t)711; /* cudaErrorTooManyPeers */
	case J9CUDA_ERROR_HOST_MEMORY_ALREADY_REGISTERED:
		return (cudaError_t)712; /* cudaErrorHostMemoryAlreadyRegistered */
	case J9CUDA_ERROR_HOST_MEMORY_NOT_REGISTERED:
		return (cudaError_t)713; /* cudaErrorHostMemoryNotRegistered */
	case J9CUDA_ERROR_OPERATING_SYSTEM:
		return (cudaError_t)304; /* cudaErrorOperatingSystem */
	case J9CUDA_ERROR_PEER_ACCESS_UNSUPPORTED:
		return (cudaError_t)217; /* cudaErrorPeerAccessUnsupported */
	case J9CUDA_ERROR_NOT_PERMITTED:
		return (cudaError_t)800; /* cudaErrorNotPermitted */
	case J9CUDA_ERROR_NOT_SUPPORTED:
		return (cudaError_t)801; /* cudaErrorNotSupported */
	default:
		break;
	}
	return (cudaError_t)(int32_t)omrCode;
}
