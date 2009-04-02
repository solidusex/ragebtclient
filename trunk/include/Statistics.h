/*
* The Rage Library
* Copyright (c) 2007 by Solidus
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* provided that the above copyright notice appear in all copies and
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*
*/
#pragma once

#include "TypeDef.h"


namespace Rage {




class RageAPI Statistics
{
private:
		enum { history = 10 };
private:
		// history of download/upload speeds a few seconds back
		t_uint32 m_download_rate_history[history];
		t_uint32 m_upload_rate_history[history];

		t_uint32 m_download_payload_rate_history[history];
		t_uint32 m_upload_payload_rate_history[history];
private:
		// the accumulators we are adding the downloads/uploads
		// to this second. This only counts the actual payload
		// and ignores the bytes sent as protocol chatter.
		t_uint32 m_downloaded_payload;
		t_uint32 m_uploaded_payload;

		// the accumulators we are adding the downloads/uploads
		// to this second. This only counts the protocol
		// chatter and ignores the actual payload
		t_uint32 m_downloaded_protocol;
		t_uint32 m_uploaded_protocol;
private:
		// total download/upload counters
		// only counting payload data
		t_uint64 m_total_download_payload;
		t_uint64 m_total_upload_payload;

		// total download/upload counters
		// only counting protocol chatter
		t_uint64 m_total_download_protocol;
		t_uint64 m_total_upload_protocol;
private:
		// current mean download/upload rates
		float m_mean_download_rate;
		float m_mean_upload_rate;

		float m_mean_download_payload_rate;
		float m_mean_upload_payload_rate;
public:
		Statistics();
		void operator+=(const Statistics& s);
		Statistics(const Statistics& other);
		Statistics& operator=(const Statistics& s);
public:
		void ReceivedBytes(size_t bytes_payload, size_t bytes_protocol);

		void SentBytes(size_t bytes_payload, size_t bytes_protocol);
		
		float UploadRate()const;
		float DownloadRate()const;

		float UploadPayloadRate()const;
		float DownloadPayloadRate()const;

		t_uint64 TotalPayloadUpload()const;
		t_uint64 TotalPayloadDownload()const;

		t_uint64 TotalProtocolUpload() const;
		t_uint64 TotalProtocolDownload()const;

		// should be called once every second
		void SecondTick();

		void Reset();

};

















}