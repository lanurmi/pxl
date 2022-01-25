#ifdef PXL_VIDEO_DUMMY

pxl::video::EncoderRef pxl::video::createEncoder(const EncoderInfo&)
{
	return nullptr;	
}

pxl::video::DecoderRef pxl::video::createDecoder() {
	return nullptr;
}

pxl::video::FrameRef createFrame(const pxl::Image&, pxl::i64)
{
	return nullptr;
}

#endif
