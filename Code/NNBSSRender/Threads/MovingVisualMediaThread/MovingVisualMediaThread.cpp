///////////////////////////////////////////////////////////////////////////////
// Name:        MovingVisualMediaThread.cpp
// Purpose:     Thread and event for retrieving images from a camera with cv::VideoCapture
// Author:      Orkhan Aliyev
// Created:     2022-01-01
// Copyright:   (c) 2022 Orkhan Aliyev
/////////////////////////////////////////////////////////////////////////

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>
// NNBSS Render
#include "MovingVisualMediaThread.h"
#include <NNBSSRender/Convertors/convertmattowxbmp.h>
// Standard C Lib
#include <filesystem>
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>

#if !wxUSE_THREADS
#error "Thread support is required!"
#endif // wxUSE_THREADS

/***********************************************************************************************

	MovingVisualMediaEvent

***********************************************************************************************/

// See header for more information
	// Device Events
wxDEFINE_EVENT(EVT_MVM_DEVICE_CONNECT_ATTEMPT, MovingVisualMediaEvent);
wxDEFINE_EVENT(EVT_MVM_DEVICE_CONNECT_FAIL, MovingVisualMediaEvent);
wxDEFINE_EVENT(EVT_MVM_DEVICE_CONNECTED, MovingVisualMediaEvent);
wxDEFINE_EVENT(EVT_MVM_DEVICE_HIGH_CONNECTION, MovingVisualMediaEvent);
wxDEFINE_EVENT(EVT_MVM_DEVICE_LOW_CONNECTION, MovingVisualMediaEvent);
wxDEFINE_EVENT(EVT_MVM_DEVICE_DISCONNECTED, MovingVisualMediaEvent);
// Thumbnail Events
wxDEFINE_EVENT(EVT_MVM_CAPTURE_STARTED, MovingVisualMediaEvent);
wxDEFINE_EVENT(EVT_MVM_COMMAND_RESULT, MovingVisualMediaEvent);
wxDEFINE_EVENT(EVT_MVM_ERROR_OPEN, MovingVisualMediaEvent);
wxDEFINE_EVENT(EVT_MVM_ERROR_EMPTY, MovingVisualMediaEvent);
wxDEFINE_EVENT(EVT_MVM_ERROR_EXCEPTION, MovingVisualMediaEvent);

/***********************************************************************************************

	CNNBSSMovingVisualMediaThread

***********************************************************************************************/

struct SNNBSSMovingVisualMediaThreadInitList// Init list 
{
	CNNBSSStreamSender* p_Stream{ nullptr };
	CNNBSSStreamReceiver* p_StreamReceiver{ nullptr };

	// MVM Writer
	CNNBSSMovingVisualMediaWriter* p_MVMWriter{ nullptr };

	// ImgProc
	CNNBSSImgProcBrightnessContrast m_BrightnessContrast;
	CNNBSSImgProcHistogram m_Histogram;

	//---------------------------------------------------
	MovingVisualMediaEvent* evt{ nullptr };
	wxLongLong frameCaptureStartedTime;

	cv::Mat      MainFrameMatrix;
	const unsigned int _FIXSCALE = 512;
	long         msPerFrame;
	int m_CurFPS = 0;

	// Temporal Detection Model is used to update when Detection Model has been modified/changed
	std::vector<NNBSSDetectionModel::SCNNBSSDeviceAssignedDetectionModels> Queue_SCNNBSSDeviceAssignedDetectionModelsList;
};

CNNBSSMovingVisualMediaThread::CNNBSSMovingVisualMediaThread(SNNBSSMVMData* params)
	: wxThread(wxTHREAD_JOINABLE), m_Init(new SNNBSSMovingVisualMediaThreadInitList), m_Parameters(params)
{
}

CNNBSSMovingVisualMediaThread::~CNNBSSMovingVisualMediaThread()
{
	if (m_Init->p_MVMWriter)
		nnbsstd::NoDangling(m_Init->p_MVMWriter);

	if (m_Init)
		nnbsstd::NoDangling(m_Init);
}

int CNNBSSMovingVisualMediaThread::InitCapture()
{
	int _return = CT_INVALID;

	unsigned long cameraIndex = 0;

	try
	{
		if (m_Parameters->m_MVMSourceAddress.ToCULong(&cameraIndex))
		{
			p_VideoCapture.reset(new cv::VideoCapture(cameraIndex, cv::CAP_ANY));

			_return = CT_OPENCV_USB;
		}
		else
		{
			p_VideoCapture.reset(new cv::VideoCapture(m_Parameters->m_MVMSourceAddress.ToStdString().c_str(), cv::CAP_ANY));

			// One more thing. Since user may enter a file path and OpenCV welcomes that,
			// We'll use a trick to check whether it's a file path or IP address;
			if (!std::filesystem::exists(m_Parameters->m_MVMSourceAddress.ToStdString().c_str()))
			{
				_return = CT_OPENCV_IP;// it's an IP address
			}
			else
			{
				_return = CT_OPENCV_FILE;// It's a video file
			}
		}

		if (!p_VideoCapture->isOpened())// If OpenCV could not connect the address, we'll try another option which we call NNBSS connection
		{
			m_Init->p_StreamReceiver = new CNNBSSStreamReceiver(m_Parameters);
			bool _result = m_Init->p_StreamReceiver->Setup();

			if (!_result)// If this condition is true, we have no valid address it means.
				_return = CT_INVALID;// to prevent any unexpected result, I set this again.
			else// If this condition is true, we have a NNBSS connection
			{
				_return = CT_NNBSS_IP;
			}
		}
	}
	catch (std::exception& e)
	{
		NNBSSMessageBoxSimple(_("Error"), wxString::Format("%s", e.what()), wxICON_ERROR);

		_return = CT_INVALID;// to prevent any unexpected result, I set this again.
	}
	catch (...)
	{
		NNBSSMessageBoxSimple(_("Error"), wxString::Format("Unknown exception, %s device could not be connected!", m_Parameters->m_MVMName), wxICON_ERROR);

		_return = CT_INVALID;// to prevent any unexpected result, I set this again.
	}

	return _return;
}

wxThread::ExitCode CNNBSSMovingVisualMediaThread::Entry()
{
#if wxCHECK_VERSION(3, 1, 6)
	SetName(wxString::Format("CameraThread %s", GetPanelName()));
#endif

	cv::setUseOptimized(true);
	cv::setBreakOnError(false);// Don't turn on!

	//auto PostBitmapPanelEvent = [&](MovingVisualMediaEvent* event)
	//{
	//	m_isCapturing = false;

	//	for (auto& props : m_cameraPropertiesData)
	//	{
	//		//m_Init->evt = new MovingVisualMediaEvent(EVT_MVM_ERROR_EXCEPTION, props);
	//		// m_Init->evt->SetString(wxString::Format("%s", message));
	//		// props->GetEventSinkObject()->QueueEvent(m_Init->evt);

	//		//m_Parameters->p_GlobalEventSink->QueueEvent(new MovingVisualMediaEvent(EVT_MVM_ERROR_EXCEPTION, props, m_Parameters));
	//	}

	//	//m_Parameters->p_GlobalEventSink->QueueEvent(new MovingVisualMediaEvent(EVT_MVM_DEVICE_DISCONNECTED, nullptr, m_Parameters));
	//};


	m_Parameters->p_GlobalEventSink->QueueEvent(new MovingVisualMediaEvent(EVT_MVM_DEVICE_CONNECT_ATTEMPT, m_Parameters));

	// Try to connect
	m_DATA_DECODER_TYPE = InitCapture();
	if (m_DATA_DECODER_TYPE == CT_INVALID /*Invalid address, could not be connected*/)
	{
		// Use send event lambda function instead?

		/*for (auto& props : m_cameraPropertiesData)
		{
			props->GetEventSinkObject()->QueueEvent(new MovingVisualMediaEvent(EVT_MVM_ERROR_OPEN, props));
			m_Parameters->p_GlobalEventSink->QueueEvent(new MovingVisualMediaEvent(EVT_MVM_ERROR_OPEN, props, m_Parameters));
		}*/

		//m_Parameters->p_GlobalEventSink->QueueEvent(new MovingVisualMediaEvent(EVT_MVM_DEVICE_CONNECT_FAIL, nullptr, m_Parameters));

		return static_cast<wxThread::ExitCode>(nullptr);
	}

	m_isCapturing = true;

	{
		//// Set default FPS
		//// TODO: same functionality should be for NNBSS Connection
		//if (m_DATA_DECODER_TYPE == CT_OPENCV_USB ||
		//	m_DATA_DECODER_TYPE == CT_OPENCV_IP  ||
		//	m_DATA_DECODER_TYPE == CT_OPENCV_FILE )
		//	m_Init->m_CurFPS = p_VideoCapture->get(cv::CAP_PROP_FPS);

		// Video Recorder
		m_Init->p_MVMWriter = new CNNBSSMovingVisualMediaWriter;
	}

	// Whether it's a video file
	if (m_Parameters->m_MVMType == NNBSS_MVM_TYPE_VIDEO && m_DATA_DECODER_TYPE == CT_OPENCV_FILE)
	{
		p_VideoManager.SetMax((const unsigned long long)p_VideoCapture->get(cv::CAP_PROP_FRAME_COUNT));
		p_VideoManager.SetPaused(true);// set to true later
		p_VideoManager.SetCurrent(0ULL);
		p_VideoManager.SetLoop(true);
	}
	else if (m_Parameters->m_MVMType != NNBSS_MVM_TYPE_VIDEO && m_DATA_DECODER_TYPE == CT_OPENCV_FILE
		|| m_Parameters->m_MVMType == NNBSS_MVM_TYPE_VIDEO && m_DATA_DECODER_TYPE != CT_OPENCV_FILE)
	{
		NNBSSMessageBoxSimple(_("Error"), _("Somethings went wrong! Error Code: MVM_INVALID_THREAD_TYPE"));
		//CriticError("Somethings went wrong! Error Code: MVM_INVALID_THREAD_TYPE");
	}

	m_Parameters->p_GlobalEventSink->QueueEvent(new MovingVisualMediaEvent(EVT_MVM_DEVICE_CONNECTED, m_Parameters));

	while (!TestDestroy())
	{
		try
		{
			(*p_VideoCapture) >> m_Init->MainFrameMatrix;

			m_Init->frameCaptureStartedTime = wxGetUTCTimeMillis();

			// Only for videos
			if (m_DATA_DECODER_TYPE == CT_OPENCV_FILE)
			{
				if (p_VideoManager.GetPaused())
				{
					p_VideoCapture->set(cv::CAP_PROP_POS_FRAMES, (double)p_VideoManager.GetCurrent());
				}
				else
				{
					// Frame increment
					if (_GetCurrentFrame() >= _GetFrameCount())
					{
						// Loop video or not?
						if (p_VideoManager.GetLoop())
							p_VideoCapture->set(cv::CAP_PROP_POS_FRAMES, 0);
						else
						{
							_PauseAt(_GetFrameCount());
						}
					}
					else
					{
						p_VideoManager.SetCurrent(p_VideoManager.GetCurrent() + 1ULL);
					}
				}

			}

			// TODO: same functionality should be for NNBSS Connection
			//if (m_DATA_DECODER_TYPE == CT_NNBSS_IP)
			//	m_Init->MainFrameMatrix = m_Init->p_StreamReceiver->GetCurrentFrame();
			//else
			//{
			//	(*p_VideoCapture) >> m_Init->MainFrameMatrix;
			//}

			{
				// TODO: same functionality should be for NNBSS Connection
				if (m_DATA_DECODER_TYPE == CT_OPENCV_USB ||
					m_DATA_DECODER_TYPE == CT_OPENCV_IP ||
					m_DATA_DECODER_TYPE == CT_OPENCV_FILE) {

					// Resolution
					if (m_Parameters->m_MVMUseDefault)
					{
						if (m_Parameters->m_MVMWidth <= 0)
							m_Parameters->m_MVMWidth = p_VideoCapture->get(cv::CAP_PROP_FRAME_WIDTH);

						if (m_Parameters->m_MVMHeight <= 0)
							m_Parameters->m_MVMHeight = p_VideoCapture->get(cv::CAP_PROP_FRAME_HEIGHT);

						cv::resize(m_Init->MainFrameMatrix, m_Init->MainFrameMatrix, cv::Size(std::clamp(m_Parameters->m_MVMWidth, 1, NNBSSMAXELEMENT),
							std::clamp(m_Parameters->m_MVMHeight, 1, NNBSSMAXELEMENT)));
					}
				}
			}

			{
				// TODO: same functionality should be for NNBSS Connection
				if (m_DATA_DECODER_TYPE == CT_OPENCV_USB ||
					m_DATA_DECODER_TYPE == CT_OPENCV_IP ||
					m_DATA_DECODER_TYPE == CT_OPENCV_FILE) {
					// FPS
					if (m_Parameters->m_MVMFPS != m_Init->m_CurFPS)
					{
						if (m_Parameters->m_MVMFPS <= 0 || m_Parameters->m_MVMFPS > 1000)
							m_Parameters->m_MVMFPS = 25;

						p_VideoCapture->set(cv::CAP_PROP_FPS, m_Parameters->m_MVMFPS);

						m_Init->m_CurFPS = m_Parameters->m_MVMFPS;
					}
					// Then limit FPS out of condition so we may not get 0 or n>1000 fps
					std::clamp(m_Parameters->m_MVMFPS, 1, 1000);
				}
			}

			{
				// Flip handle
				if (m_Parameters->m_ImageProcessingFlipHorizontal)
				{
					cv::flip(m_Init->MainFrameMatrix, m_Init->MainFrameMatrix, 1);
				}
				if (m_Parameters->m_ImageProcessingFlipVertical)
				{
					cv::flip(m_Init->MainFrameMatrix, m_Init->MainFrameMatrix, 0);
				}
			}

			{
				if (m_Parameters->m_Streaming)
				{
					if (!m_Init->p_Stream)// Init properly if is not created
					{
						m_Init->p_Stream = new CNNBSSStreamSender(m_Parameters);
						if (m_Init->p_Stream->Start() != wxTHREAD_NO_ERROR)
						{
							NNBSSErrorShow("Error, can't start streaming!", 100);
						}
					}

					m_Init->p_Stream->AddToCacheQueue(m_Init->MainFrameMatrix);
				}
				else
				{
					if (m_Init->p_Stream)
					{
						m_Init->p_Stream->Delete();
						delete m_Init->p_Stream;
						m_Init->p_Stream = nullptr;
					}
				}
			}


			{
				// Video record
				if (m_Parameters->m_Record)
				{
					if (!m_Init->p_MVMWriter->IsRecording())
					{
						// TODO: File format and name should not be hardcoded
						m_Init->p_MVMWriter->Record(cv::String(NNBSS_DIR__PLAYBACK_DIR.ToStdString() + m_Parameters->m_MVMName + ".avi")
							, m_Parameters->m_RecordResolutionWidth, m_Parameters->m_RecordResolutionHeight,
							m_Parameters->m_RecordFPS,
							m_Parameters->m_RecordEncoderName);
					}
					else
					{
						m_Init->p_MVMWriter->AddToCacheQueue(m_Init->MainFrameMatrix);
					}
				}
				else
				{
					if (m_Init->p_MVMWriter->IsRecording())
					{
						m_Init->p_MVMWriter->StopRecord();
					}
				}
			}

			
			{// General Effects on Main Mat(Local effects are only applied to thumbnails[that means they won't be saved])

				// Show histogram
				if (m_Parameters->m_ImageProcessingShowHistogram)
					m_Init->m_Histogram.DisplayHistogramOn(m_Init->MainFrameMatrix, m_Init->MainFrameMatrix);
			}

			for (auto& BitmapPanel : GetBitmapPanelPool())
			{
				wxCriticalSectionLocker locker(BitmapPanel->m_CriticalSection);// I think Critical Section could be stored in thread

				const wxSize BitmapPanelSize = BitmapPanel->GetSize();

				NNBSSThumbnailFrameDataUPTR frameData(new CNNBSSThumbnailFrameData());
				wxLongLong         frameCaptureStartedTime;
				SNNBSSThumbnailCommandData  commandData;

				frameCaptureStartedTime = wxGetUTCTimeMillis();

				if (BitmapPanel->m_CommandData.ReceiveTimeout(0, commandData) == wxMSGQUEUE_NO_ERROR)
					ProcessCameraCommand(commandData);

				if (!m_Init->MainFrameMatrix.empty())
				{
					frameData->SetFrame(new wxBitmap(m_Init->MainFrameMatrix.cols, m_Init->MainFrameMatrix.rows, 24));
					ConvertMatBitmapTowxBitmap(m_Init->MainFrameMatrix, *frameData->GetFrame());

					{
						// Thumbnail specials

						{
							int _CurrentDetectionCount = PerformDetection(m_Init->MainFrameMatrix);
						}

						if (m_Parameters->m_ImageProcessingActivate)
						{
							m_Init->m_BrightnessContrast.BrightnessContrast(m_Init->MainFrameMatrix,
								m_Parameters->m_ImageProcessingBrightness, m_Parameters->m_ImageProcessingContrast);
						}

						//cv::resize(m_Init->MainFrameMatrix, matThumbnail, cv::Size(BitmapPanelSize.GetWidth(), BitmapPanelSize.GetHeight()));
						{
							ResizeKeepAspectRatio(m_Init->MainFrameMatrix, m_Init->MainFrameMatrix, cv::Size(BitmapPanelSize.GetWidth(),
								BitmapPanelSize.GetHeight()),
								cv::Scalar(0, 0, 0));
						}
						frameData->SetThumbnail(new wxBitmap(BitmapPanelSize, 24));
						ConvertMatBitmapTowxBitmap(m_Init->MainFrameMatrix, *frameData->GetThumbnail());
					}

					{
						//wxCriticalSectionLocker locker(*m_cameraSetupData.framesCS);

						BitmapPanel->m_FrameData.push_back(std::move(frameData));
					}

					m_Init->MainFrameMatrix.release();
				}
			}

			// Time vars
			m_Init->msPerFrame = 1000 / std::clamp(m_Parameters->m_MVMFPS, 1, 1000);
			const wxLongLong timeSinceFrameCaptureStarted = wxGetUTCTimeMillis() - m_Init->frameCaptureStartedTime;
			const long       timeToSleep = m_Init->msPerFrame - timeSinceFrameCaptureStarted.GetLo();
			//  SLEEP_HANDLE
			{
				if (m_Parameters->m_MVMFPS > 0)
				{
					if (timeToSleep > 0)
						Sleep(timeToSleep);
				}
				else if (m_Parameters->m_MVMFPS == 0)
				{
					//do nothing
				}
				else if (m_Parameters->m_MVMFPS < 0 || m_Parameters->m_MVMFPS > 1000)
				{
					m_Parameters->m_MVMFPS = 25;
				}

			}

			// TODO: Connection level is based on FPS. This method may give wrong results if: ~30<=FPS>=0!
			if (std::clamp((int)(timeToSleep * (-1)), 0, NNBSSMAXELEMENT) > 600)
			{
				if (m_LastDCL != DCL_LOW)
				{
					m_Parameters->p_GlobalEventSink->QueueEvent(new MovingVisualMediaEvent(EVT_MVM_DEVICE_LOW_CONNECTION, m_Parameters));
					m_LastDCL = DCL_LOW;
				}
			}
			else
			{
				if (m_LastDCL != DCL_HIGH)
				{
					m_Parameters->p_GlobalEventSink->QueueEvent(new MovingVisualMediaEvent(EVT_MVM_DEVICE_HIGH_CONNECTION, m_Parameters));
					m_LastDCL = DCL_HIGH;
				}
			}

		}
		catch (cv::Exception& e)
		{
			//CriticError(std::string(e.what()) + "\n" + std::string(e.msg));

			wxLogMessage("Error Reason: %s", e.what());

			break;
		}
		catch (const std::exception& e)
		{
			//CriticError(e.what());

			wxLogMessage("Error Reason: %s", e.what());

			break;
		}
		catch (...)
		{
			//CriticError("Unknown Error");

			wxLogMessage("Unknown");

			break;
		}
	}

	return static_cast<wxThread::ExitCode>(nullptr);
}

//--------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
//  ---------------------------------------------------------------------------------------------
//   -------------------------------------------------------------------------------------------
//    -----------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------


void CNNBSSMovingVisualMediaThread::ResizeKeepAspectRatio(const cv::Mat& src, cv::Mat& dst, const cv::Size& dstSize, const cv::Scalar& backgroundColor = {})
{
	// Don't handle anything in this corner case
	if (dstSize.width <= 0 || dstSize.height <= 0)
		return;

	// Not job is needed here, let's avoid any copy
	if (src.cols == dstSize.width && src.rows == dstSize.height)
	{
		dst = src;
		return;
	}

	// Try not to reallocate memory if possible
	cv::Mat output = [&]()
	{
		if (dst.data != src.data && dst.cols == dstSize.width && dst.rows == dstSize.height && dst.type() == src.type())
			return dst;
		return cv::Mat(dstSize.height, dstSize.width, src.type(), backgroundColor);
	}();

	// 'src' inside 'dst'
	const auto imageBox = [&]()
	{
		const auto h1 = int(dstSize.width * (src.rows / (double)src.cols));
		const auto w2 = int(dstSize.height * (src.cols / (double)src.rows));

		const bool horizontal = h1 <= dstSize.height;

		const auto width = horizontal ? dstSize.width : w2;
		const auto height = horizontal ? h1 : dstSize.height;

		const auto x = horizontal ? 0 : int(double(dstSize.width - width) / 2.);
		const auto y = horizontal ? int(double(dstSize.height - height) / 2.) : 0;

		return cv::Rect(x, y, width, height);
	}();

	cv::Rect firstBox;
	cv::Rect secondBox;

	if (imageBox.width > imageBox.height)
	{
		firstBox.x = 0;
		firstBox.width = dstSize.width;
		firstBox.y = 0;
		firstBox.height = imageBox.y;

		secondBox.x = 0;
		secondBox.width = dstSize.width;
		secondBox.y = imageBox.y + imageBox.height;
		secondBox.height = dstSize.height - secondBox.y;
	}
	else
	{
		firstBox.y = 0;
		firstBox.height = dstSize.height;
		firstBox.x = 0;
		firstBox.width = imageBox.x;

		secondBox.y = 0;
		secondBox.height = dstSize.height;
		secondBox.x = imageBox.x + imageBox.width;
		secondBox.width = dstSize.width - secondBox.x;
	}

	// Resizing to final image avoid useless memory allocation
	cv::Mat outputImage = output(imageBox);
	assert(outputImage.cols == imageBox.width);
	assert(outputImage.rows == imageBox.height);
	const auto* dataBeforeResize = outputImage.data;

	cv::resize(src, outputImage, cv::Size(outputImage.cols, outputImage.rows));

	assert(dataBeforeResize == outputImage.data);

	const auto drawBox = [&](const cv::Rect& box)
	{
		if (box.width > 0 && box.height > 0)
		{
			cv::rectangle(output, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), backgroundColor, -1);
		}
	};

	drawBox(firstBox);
	drawBox(secondBox);

	// Finally copy output to dst, like that user can use src & dst to the same cv::Mat
	dst = output;
}

int CNNBSSMovingVisualMediaThread::PerformDetection(cv::Mat& output)
{
	int _RectangleCount = 0;

	// since this function is a bit expensive, we'll do some condition stuff here not to load detection models continously
	// The main aim is update assigned detection models and also if some are unassigned
	if (NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList != m_Init->Queue_SCNNBSSDeviceAssignedDetectionModelsList)
	{
		// Clear detector base array(note: p_MainDetectorBase array is thread special and not defined outside of thread)
		p_MainDetectorBase.clear();

		// Add detector if any is assigned to the device
		if (!NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList.empty())
		{
			for (int c = 0; c < (int)NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList.size(); c++)
			{
				if (NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[c].Address == m_Parameters->m_MVMSourceAddress
					&& NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[c].DeviceName == m_Parameters->m_MVMName)
				{
					// if the order is changed, clear the p_MainDetectorBase array, and add it again
					p_MainDetectorBase.emplace_back(new CNNBSSAIObjectDetection());
				}
			}
		}

		//Load Detecton models(CascadeClassifier load only for now)
		for (int c = 0; c < (int)p_MainDetectorBase.size(); c++)
		{
			for (int t = 0; t < (int)NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList.size(); t++)
			{
				if (NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].Address == m_Parameters->m_MVMSourceAddress
					&& NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].DeviceName == m_Parameters->m_MVMName)
				{
					// use if condition
					p_MainDetectorBase[c]->LoadCascadeClassifier(cv::String(NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[c]._AssignedModelPath));
				}
			}
		}

		// And make temporal list equal to current changed
		m_Init->Queue_SCNNBSSDeviceAssignedDetectionModelsList = NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList;
	}

	// Detect and draw
	if (!p_MainDetectorBase.empty())
	{
		for (int c = 0; c < (int)p_MainDetectorBase.size(); c++)
		{
			for (int t = 0; t < (int)NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList.size(); t++)
			{
				if (NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].Works
					&& NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].Address == m_Parameters->m_MVMSourceAddress
					&& NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].DeviceName == m_Parameters->m_MVMName)
				{
					p_MainDetectorBase[c]->SetParameters(
						NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].m_arrayScaleFactor,
						NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].m_scaleFactor,
						NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].m_minNeighbors,
						cv::Size(NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].m_minSize.GetWidth(),
							NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].m_minSize.GetHeight()),
						cv::Size(NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].m_maxSize.GetWidth(),
							NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t].m_maxSize.GetHeight()));

					std::vector<cv::Rect> _ROI;// All Regions Of Interest

					try
					{
						_ROI = p_MainDetectorBase[c]->Detect(output, NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t]._AssignedModelPath);

						_RectangleCount = _ROI.size();
						// Drawing
						for (int c = 0; c < _ROI.size(); c++)
						{
							cv::Mat ROIThumbnail = output(_ROI[c]);
							cvtColor(ROIThumbnail, ROIThumbnail, cv::COLOR_BGR2GRAY);
							cvtColor(ROIThumbnail, ROIThumbnail, cv::COLOR_GRAY2BGR);

							{
								if (1)
								{
									// Notifications
									// Move to respective places. I don't think nested use is the best way
									NNBSSNotifications::SNNBSSNotificationAttributes m_NotParams;
									m_NotParams.ROI = cv::Mat(output(_ROI[c]));// Save cropped original mat without channel changes
									m_NotParams.DetectionName = wxString::Format("%s",
										NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t]._AssignedModelPath);
									m_NotParams.Message = NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t]._NotificationMessage;
									m_NotParams.AdditionalNotes = NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t]._AdditionalUserNotes;

									NNBSSNotifications::AppendNotification(m_NotParams);
								}
							}

							cv::Scalar detectionColour(255, 180, 0);
							{
								// ROI drawing
								const int cornerLength = 50;
								const int cornerThickness = 5;
								// Rectangle corners
								cv::rectangle(output, _ROI[c], detectionColour, 2);
								cv::line(output, cv::Point(_ROI[c].tl().x, _ROI[c].tl().y), cv::Point(_ROI[c].tl().x + cornerLength, _ROI[c].tl().y), detectionColour, cornerThickness);
								cv::line(output, cv::Point(_ROI[c].tl().x, _ROI[c].tl().y), cv::Point(_ROI[c].tl().x, _ROI[c].tl().y + cornerLength), detectionColour, cornerThickness);
								cv::line(output, cv::Point(_ROI[c].br().x, _ROI[c].br().y), cv::Point(_ROI[c].br().x - cornerLength, _ROI[c].br().y), detectionColour, cornerThickness);
								cv::line(output, cv::Point(_ROI[c].br().x, _ROI[c].br().y), cv::Point(_ROI[c].br().x, _ROI[c].br().y - cornerLength), detectionColour, cornerThickness);

								if (!NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t]._Label.empty())
								{
									std::string m_DLabel = NNBSSDetectionModel::SCNNBSSAssignedDetectionModelsList[t]._Label.ToStdString();
									if (m_DLabel.size() > 10)
									{
										std::string buffer;
										for (int c = 0; c < 10; c++)
										{
											buffer += m_DLabel[c];
										}
										m_DLabel = buffer + "...";
									}

									cv::rectangle(output, _ROI[c].tl() - cv::Point(0, 10), _ROI[c].tl() + cv::Point((m_DLabel.size() + 1) * 19, -50), detectionColour, cv::FILLED);
									detectionColour = cv::Scalar(255, 255, 255);
									cv::putText(output, m_DLabel,
										cv::Point(_ROI[c].tl().x, _ROI[c].tl().y - 20), cv::FONT_HERSHEY_DUPLEX, 1, detectionColour, 2);
								}
							}

							//// Display Additional Information
							cv::Rect ROIThumbnailArea = _ROI[c];
							ROIThumbnailArea.width = ROIThumbnailArea.width / 3;
							ROIThumbnailArea.height = ROIThumbnailArea.height / 3;
							ROIThumbnailArea.x = _ROI[c].x + _ROI[c].width;
							ROIThumbnailArea.y = _ROI[c].y;

							//	{
							//		// Face recognition
							//		static bool OnceGatee = false;
							//		if (!OnceGatee)
							//		{
							//			p_MainFaceRecognition = new CNNBSSAIFaceRecognition();
							//			p_MainFaceRecognition->Init();
							//			OnceGatee = true;
							//		}
							//		int predictedLabel = -1;
							//		double confidence = 1000.0;
							//		SNNBSSPersonData personData;
							//		p_MainFaceRecognition->PerformFaceRecognition(ROIThumbnail, predictedLabel, confidence, personData);

							//		constexpr int _Distance = 15;

							//		std::string PersonSurfaceInfo = personData.Name + " " + personData.Surename + " " + personData.FatherName;
							//		detectionColour = cv::Scalar(255, 180, 0);
							//		cv::rectangle(output, ROIThumbnailArea.tl(), ROIThumbnailArea.tl() + cv::Point((PersonSurfaceInfo.size() + 1) * 9, 20), detectionColour, cv::FILLED);
							//		detectionColour = cv::Scalar(255, 255, 255);
							//		cv::putText(output, PersonSurfaceInfo,
							//			cv::Point(ROIThumbnailArea.x, ROIThumbnailArea.y + _Distance),
							//			cv::FONT_HERSHEY_DUPLEX, 0.5, detectionColour, 0.5);

							//		//// test
							//		//static int _ONCEGate = 100;
							//		//if (_ONCEGate < 10)
							//		//{
							//		//	_ONCEGate++;
							//		//}
							//		//else
							//		//{
							//		//	SNNBSSPersonData m_PersonParams;
							//		//	m_PersonParams.Name = "Abdulov";
							//		//	m_PersonParams.Surename = "Kanan";
							//		//	m_PersonParams.FatherName = "Zaur";
							//		//	m_PersonParams.Age = "23";
							//		//	m_PersonParams.Gender = "Male";
							//		//	m_PersonParams.Length = "1.75";
							//		//	p_MainFaceRecognition->RequestWriteCache(ROIThumbnail, m_PersonParams);
							//		//	_ONCEGate = 0;
							//		//}
							//	}

							//	//if (!(ROIThumbnailArea.x <= 0 || ROIThumbnailArea.x >= output.cols ||
							//	//	ROIThumbnailArea.y <= 0 || ROIThumbnailArea.y >= output.rows))
							//	//{
							//	//	resize(ROIThumbnail, ROIThumbnail, cv::Size(ROIThumbnailArea.width, ROIThumbnailArea.height));

							//	//	//	// Then draw a line
							//	//	cv::line(output, cv::Point(_ROI[c].tl().x + _ROI[c].width, _ROI[c].y),
							//	//		cv::Point(ROIThumbnailArea.tl().x, ROIThumbnailArea.tl().y), cv::Scalar(100, 200, 100), 1);

							//	//	ROIThumbnail.copyTo(output(ROIThumbnailArea));
							//	//	// Then draw a rectangle that encompasses ROI
							//	//	cv::rectangle(output, ROIThumbnailArea, cv::Scalar(100, 200, 100), 2);
							//	//}

						}
					}
					catch (const std::exception& e)
					{
						NNBSSErrorShow(wxString::Format("%s", e.what()).ToStdString().c_str(), 100);
						break;
					}
					catch (...)
					{
						NNBSSErrorShow("Unknown exception is thrown while detecting!", 100);
						break;
					}
				}
			}
		}
	}

	return _RectangleCount;
}

const unsigned long long CNNBSSMovingVisualMediaThread::_GetFrameCount() const
{
	return p_VideoManager.GetMax();
}

const unsigned long long CNNBSSMovingVisualMediaThread::_GetCurrentFrame() const
{
	return p_VideoManager.GetCurrent();
}

bool CNNBSSMovingVisualMediaThread::_PauseAt(const unsigned long long pauseAt)
{
	p_VideoManager.SetCurrent(pauseAt);
	p_VideoManager.SetPaused(true);

	return true;
}

bool CNNBSSMovingVisualMediaThread::_IsPaused()const
{
	return p_VideoManager.GetPaused();
}

bool CNNBSSMovingVisualMediaThread::_IsLooped() const
{
	return p_VideoManager.GetLoop();
}

void CNNBSSMovingVisualMediaThread::_SetFrameNumber(const unsigned long long frameNumber)
{
	p_VideoManager.SetCurrent(frameNumber);
}

void CNNBSSMovingVisualMediaThread::_SetFrameNumberAndPause(const unsigned long long frameNumber)
{
	_PauseAt(frameNumber);
}

void CNNBSSMovingVisualMediaThread::_Play()
{
	if (_IsPaused())
	{
		if (_GetCurrentFrame() == _GetFrameCount())// not sure if I should do -1?
		{
			// take to 0 if play is pressed at last position
			_Stop();
		}

		Sleep(100);// Unfortunately, we need a little delay here because of the sleep time of thread. I'm open to any better solution!

		p_VideoManager.SetPaused(false);
	}
}

void CNNBSSMovingVisualMediaThread::_Stop()
{
	_PauseAt(0);
}

void CNNBSSMovingVisualMediaThread::_NextFrame()
{
	if (!_IsPaused())
		return;

	if (_GetCurrentFrame() < _GetFrameCount())// don't let at the end of the video
		_PauseAt(_GetCurrentFrame() + 1);
}

void CNNBSSMovingVisualMediaThread::_PreviousFrame()
{
	if (!_IsPaused())
		return;

	if (_GetCurrentFrame() > 0)// Don't let at 0 pos
		_PauseAt(_GetCurrentFrame() - 1);
}

void CNNBSSMovingVisualMediaThread::_Loop(bool loop)
{
	p_VideoManager.SetLoop(loop);
}

void CNNBSSMovingVisualMediaThread::ProcessCameraCommand(const SNNBSSThumbnailCommandData& commandData)
{

}