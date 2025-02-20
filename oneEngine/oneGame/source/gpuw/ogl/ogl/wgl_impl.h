//===============================================================================================//
//
//      1Engine autogenerated header.
//
// Please be gentle. Generated from wgl.h
//
//===============================================================================================//
#ifndef WGL_IMPL_H_
#define WGL_IMPL_H_
#ifdef _WIN32
#include "glcorearb.h"
#include "wgl.h"
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/
extern PFNWGLCREATEBUFFERREGIONARBPROC wglCreateBufferRegionARB;
extern PFNWGLDELETEBUFFERREGIONARBPROC wglDeleteBufferRegionARB;
extern PFNWGLSAVEBUFFERREGIONARBPROC wglSaveBufferRegionARB;
extern PFNWGLRESTOREBUFFERREGIONARBPROC wglRestoreBufferRegionARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB;
extern PFNWGLGETCURRENTREADDCARBPROC wglGetCurrentReadDCARB;
extern PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;
extern PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
extern PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
extern PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;
extern PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;
extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB;
extern PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB;
extern PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB;
extern PFNWGLSETSTEREOEMITTERSTATE3DLPROC wglSetStereoEmitterState3DL;
extern PFNWGLGETGPUIDSAMDPROC wglGetGPUIDsAMD;
extern PFNWGLGETGPUINFOAMDPROC wglGetGPUInfoAMD;
extern PFNWGLGETCONTEXTGPUIDAMDPROC wglGetContextGPUIDAMD;
extern PFNWGLCREATEASSOCIATEDCONTEXTAMDPROC wglCreateAssociatedContextAMD;
extern PFNWGLCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC wglCreateAssociatedContextAttribsAMD;
extern PFNWGLDELETEASSOCIATEDCONTEXTAMDPROC wglDeleteAssociatedContextAMD;
extern PFNWGLMAKEASSOCIATEDCONTEXTCURRENTAMDPROC wglMakeAssociatedContextCurrentAMD;
extern PFNWGLGETCURRENTASSOCIATEDCONTEXTAMDPROC wglGetCurrentAssociatedContextAMD;
extern PFNWGLBLITCONTEXTFRAMEBUFFERAMDPROC wglBlitContextFramebufferAMD;
extern PFNWGLCREATEDISPLAYCOLORTABLEEXTPROC wglCreateDisplayColorTableEXT;
extern PFNWGLLOADDISPLAYCOLORTABLEEXTPROC wglLoadDisplayColorTableEXT;
extern PFNWGLBINDDISPLAYCOLORTABLEEXTPROC wglBindDisplayColorTableEXT;
extern PFNWGLDESTROYDISPLAYCOLORTABLEEXTPROC wglDestroyDisplayColorTableEXT;
extern PFNWGLMAKECONTEXTCURRENTEXTPROC wglMakeContextCurrentEXT;
extern PFNWGLGETCURRENTREADDCEXTPROC wglGetCurrentReadDCEXT;
extern PFNWGLCREATEPBUFFEREXTPROC wglCreatePbufferEXT;
extern PFNWGLGETPBUFFERDCEXTPROC wglGetPbufferDCEXT;
extern PFNWGLRELEASEPBUFFERDCEXTPROC wglReleasePbufferDCEXT;
extern PFNWGLDESTROYPBUFFEREXTPROC wglDestroyPbufferEXT;
extern PFNWGLQUERYPBUFFEREXTPROC wglQueryPbufferEXT;
extern PFNWGLGETPIXELFORMATATTRIBIVEXTPROC wglGetPixelFormatAttribivEXT;
extern PFNWGLGETPIXELFORMATATTRIBFVEXTPROC wglGetPixelFormatAttribfvEXT;
extern PFNWGLCHOOSEPIXELFORMATEXTPROC wglChoosePixelFormatEXT;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
extern PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;
extern PFNWGLGETDIGITALVIDEOPARAMETERSI3DPROC wglGetDigitalVideoParametersI3D;
extern PFNWGLSETDIGITALVIDEOPARAMETERSI3DPROC wglSetDigitalVideoParametersI3D;
extern PFNWGLGETGAMMATABLEPARAMETERSI3DPROC wglGetGammaTableParametersI3D;
extern PFNWGLSETGAMMATABLEPARAMETERSI3DPROC wglSetGammaTableParametersI3D;
extern PFNWGLGETGAMMATABLEI3DPROC wglGetGammaTableI3D;
extern PFNWGLSETGAMMATABLEI3DPROC wglSetGammaTableI3D;
extern PFNWGLENABLEGENLOCKI3DPROC wglEnableGenlockI3D;
extern PFNWGLDISABLEGENLOCKI3DPROC wglDisableGenlockI3D;
extern PFNWGLISENABLEDGENLOCKI3DPROC wglIsEnabledGenlockI3D;
extern PFNWGLGENLOCKSOURCEI3DPROC wglGenlockSourceI3D;
extern PFNWGLGETGENLOCKSOURCEI3DPROC wglGetGenlockSourceI3D;
extern PFNWGLGENLOCKSOURCEEDGEI3DPROC wglGenlockSourceEdgeI3D;
extern PFNWGLGETGENLOCKSOURCEEDGEI3DPROC wglGetGenlockSourceEdgeI3D;
extern PFNWGLGENLOCKSAMPLERATEI3DPROC wglGenlockSampleRateI3D;
extern PFNWGLGETGENLOCKSAMPLERATEI3DPROC wglGetGenlockSampleRateI3D;
extern PFNWGLGENLOCKSOURCEDELAYI3DPROC wglGenlockSourceDelayI3D;
extern PFNWGLGETGENLOCKSOURCEDELAYI3DPROC wglGetGenlockSourceDelayI3D;
extern PFNWGLQUERYGENLOCKMAXSOURCEDELAYI3DPROC wglQueryGenlockMaxSourceDelayI3D;
extern PFNWGLCREATEIMAGEBUFFERI3DPROC wglCreateImageBufferI3D;
extern PFNWGLDESTROYIMAGEBUFFERI3DPROC wglDestroyImageBufferI3D;
extern PFNWGLASSOCIATEIMAGEBUFFEREVENTSI3DPROC wglAssociateImageBufferEventsI3D;
extern PFNWGLRELEASEIMAGEBUFFEREVENTSI3DPROC wglReleaseImageBufferEventsI3D;
extern PFNWGLENABLEFRAMELOCKI3DPROC wglEnableFrameLockI3D;
extern PFNWGLDISABLEFRAMELOCKI3DPROC wglDisableFrameLockI3D;
extern PFNWGLISENABLEDFRAMELOCKI3DPROC wglIsEnabledFrameLockI3D;
extern PFNWGLQUERYFRAMELOCKMASTERI3DPROC wglQueryFrameLockMasterI3D;
extern PFNWGLGETFRAMEUSAGEI3DPROC wglGetFrameUsageI3D;
extern PFNWGLBEGINFRAMETRACKINGI3DPROC wglBeginFrameTrackingI3D;
extern PFNWGLENDFRAMETRACKINGI3DPROC wglEndFrameTrackingI3D;
extern PFNWGLQUERYFRAMETRACKINGI3DPROC wglQueryFrameTrackingI3D;
extern PFNWGLDXSETRESOURCESHAREHANDLENVPROC wglDXSetResourceShareHandleNV;
extern PFNWGLDXOPENDEVICENVPROC wglDXOpenDeviceNV;
extern PFNWGLDXCLOSEDEVICENVPROC wglDXCloseDeviceNV;
extern PFNWGLDXREGISTEROBJECTNVPROC wglDXRegisterObjectNV;
extern PFNWGLDXUNREGISTEROBJECTNVPROC wglDXUnregisterObjectNV;
extern PFNWGLDXOBJECTACCESSNVPROC wglDXObjectAccessNV;
extern PFNWGLDXLOCKOBJECTSNVPROC wglDXLockObjectsNV;
extern PFNWGLDXUNLOCKOBJECTSNVPROC wglDXUnlockObjectsNV;
extern PFNWGLCOPYIMAGESUBDATANVPROC wglCopyImageSubDataNV;
extern PFNWGLDELAYBEFORESWAPNVPROC wglDelayBeforeSwapNV;
extern PFNWGLENUMGPUSNVPROC wglEnumGpusNV;
extern PFNWGLENUMGPUDEVICESNVPROC wglEnumGpuDevicesNV;
extern PFNWGLCREATEAFFINITYDCNVPROC wglCreateAffinityDCNV;
extern PFNWGLENUMGPUSFROMAFFINITYDCNVPROC wglEnumGpusFromAffinityDCNV;
extern PFNWGLDELETEDCNVPROC wglDeleteDCNV;
extern PFNWGLENUMERATEVIDEODEVICESNVPROC wglEnumerateVideoDevicesNV;
extern PFNWGLBINDVIDEODEVICENVPROC wglBindVideoDeviceNV;
extern PFNWGLQUERYCURRENTCONTEXTNVPROC wglQueryCurrentContextNV;
extern PFNWGLJOINSWAPGROUPNVPROC wglJoinSwapGroupNV;
extern PFNWGLBINDSWAPBARRIERNVPROC wglBindSwapBarrierNV;
extern PFNWGLQUERYSWAPGROUPNVPROC wglQuerySwapGroupNV;
extern PFNWGLQUERYMAXSWAPGROUPSNVPROC wglQueryMaxSwapGroupsNV;
extern PFNWGLQUERYFRAMECOUNTNVPROC wglQueryFrameCountNV;
extern PFNWGLRESETFRAMECOUNTNVPROC wglResetFrameCountNV;
extern PFNWGLFREEMEMORYNVPROC wglFreeMemoryNV;
extern PFNWGLBINDVIDEOCAPTUREDEVICENVPROC wglBindVideoCaptureDeviceNV;
extern PFNWGLENUMERATEVIDEOCAPTUREDEVICESNVPROC wglEnumerateVideoCaptureDevicesNV;
extern PFNWGLLOCKVIDEOCAPTUREDEVICENVPROC wglLockVideoCaptureDeviceNV;
extern PFNWGLQUERYVIDEOCAPTUREDEVICENVPROC wglQueryVideoCaptureDeviceNV;
extern PFNWGLRELEASEVIDEOCAPTUREDEVICENVPROC wglReleaseVideoCaptureDeviceNV;
extern PFNWGLGETVIDEODEVICENVPROC wglGetVideoDeviceNV;
extern PFNWGLRELEASEVIDEODEVICENVPROC wglReleaseVideoDeviceNV;
extern PFNWGLBINDVIDEOIMAGENVPROC wglBindVideoImageNV;
extern PFNWGLRELEASEVIDEOIMAGENVPROC wglReleaseVideoImageNV;
extern PFNWGLSENDPBUFFERTOVIDEONVPROC wglSendPbufferToVideoNV;
extern PFNWGLGETVIDEOINFONVPROC wglGetVideoInfoNV;
extern PFNWGLGETSYNCVALUESOMLPROC wglGetSyncValuesOML;
extern PFNWGLGETMSCRATEOMLPROC wglGetMscRateOML;
extern PFNWGLSWAPBUFFERSMSCOMLPROC wglSwapBuffersMscOML;
extern PFNWGLSWAPLAYERBUFFERSMSCOMLPROC wglSwapLayerBuffersMscOML;
extern PFNWGLWAITFORMSCOMLPROC wglWaitForMscOML;
extern PFNWGLWAITFORSBCOMLPROC wglWaitForSbcOML;

//  wglgLoadFunctions() : Load OpenGL binding of Windows Junk.
// This will load Win32 OpenGL extensions needed for Windows to not shit on you.
// Returns:
//  int: 0 or greater on success.
//       -1 or less on failure.
//       The absolute value refers to number of null functions.
int wglgLoadFunctions();

#ifdef __cplusplus
} /*extern "C"*/
#endif /*__cplusplus*/
#endif /*_WIN32*/
#endif//WGL_IMPL_H_
