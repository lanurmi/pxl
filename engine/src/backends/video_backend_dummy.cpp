#if PXL_VIDEO_DUMMY

pxl::video::EncoderRef pxl::video::createEncoder(const EncoderInfo&)
{
	return nullptr;	
}

pxl::video::DecoderRef pxl::video::createDecoder() {
	return nullptr;
}

#endif
