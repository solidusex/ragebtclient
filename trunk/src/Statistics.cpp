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
#include "Statistics.h"


namespace Rage {





Statistics::Statistics(const Statistics& other)
{
		*this = other;
}

Statistics& Statistics::operator=(const Statistics& other)
{
		if(this != &other)
		{
				m_downloaded_payload = other.m_downloaded_payload;
				m_total_download_payload = other.m_downloaded_payload;
				m_downloaded_protocol = other.m_downloaded_protocol;
				m_total_download_protocol = other.m_downloaded_protocol;

				m_uploaded_payload = other.m_uploaded_payload;
				m_total_upload_payload = other.m_uploaded_payload;
				m_uploaded_protocol = other.m_uploaded_protocol;
				m_total_upload_protocol = other.m_uploaded_protocol;

				std::copy(other.m_download_rate_history, other.m_download_rate_history+history, m_download_rate_history);
				std::copy(other.m_upload_rate_history, other.m_upload_rate_history+history, m_upload_rate_history);
				std::copy(other.m_download_payload_rate_history, other.m_download_payload_rate_history+history, m_download_payload_rate_history);
				std::copy(other.m_upload_payload_rate_history, other.m_upload_payload_rate_history+history, m_upload_payload_rate_history);
		}
		return *this;
}

Statistics::Statistics(): m_downloaded_payload(0)
						, m_uploaded_payload(0)
						, m_downloaded_protocol(0)
						, m_uploaded_protocol(0)
						, m_total_download_payload(0)
						, m_total_upload_payload(0)
						, m_total_download_protocol(0)
						, m_total_upload_protocol(0)
						, m_mean_download_rate(0)
						, m_mean_upload_rate(0)
						, m_mean_download_payload_rate(0)
						, m_mean_upload_payload_rate(0)
{
		std::fill(m_download_rate_history, m_download_rate_history+history, 0);
		std::fill(m_upload_rate_history, m_upload_rate_history+history, 0);
		std::fill(m_download_payload_rate_history, m_download_payload_rate_history+history, 0);
		std::fill(m_upload_payload_rate_history, m_upload_payload_rate_history+history, 0);
}

void Statistics::Reset()
{
		Statistics new_stat;
		*this = new_stat;
}

void Statistics::operator+=(const Statistics& s)
{
		//ReceivedBytes(s.m_downloaded_payload, s.m_downloaded_protocol);

		m_downloaded_payload += s.m_downloaded_payload;
			m_total_download_payload += s.m_downloaded_payload;
			m_downloaded_protocol += s.m_downloaded_protocol;
			m_total_download_protocol += s.m_downloaded_protocol;
			
			m_uploaded_payload += s.m_uploaded_payload;
			m_total_upload_payload += s.m_uploaded_payload;
			m_uploaded_protocol += s.m_uploaded_protocol;
			m_total_upload_protocol += s.m_uploaded_protocol;
			/*
		m_downloaded_payload += s.m_downloaded_payload;
		m_total_download_payload += s.m_total_download_payload;
		m_downloaded_protocol += s.m_downloaded_protocol;
		m_total_download_protocol += s.m_total_download_protocol;

		m_uploaded_payload += s.m_uploaded_payload;
		m_total_upload_payload += s.m_total_upload_payload;
		m_uploaded_protocol += s.m_uploaded_protocol;
		m_total_upload_protocol += s.m_total_upload_protocol;*/
		
}

void Statistics::ReceivedBytes(size_t bytes_payload, size_t bytes_protocol)
{
		m_downloaded_payload += bytes_payload;
		m_total_download_payload += bytes_payload;
		m_downloaded_protocol += bytes_protocol;
		m_total_download_protocol += bytes_protocol;
}

void Statistics::SentBytes(size_t bytes_payload, size_t bytes_protocol)
{
		m_uploaded_payload += bytes_payload;
		m_total_upload_payload += bytes_payload;
		m_uploaded_protocol += bytes_protocol;
		m_total_upload_protocol += bytes_protocol;
}



float Statistics::UploadRate() const { return m_mean_upload_rate; }
float Statistics::DownloadRate() const { return m_mean_download_rate; }

float Statistics::UploadPayloadRate() const { return m_mean_upload_payload_rate; }
float Statistics::DownloadPayloadRate() const { return m_mean_download_payload_rate; }

t_uint64 Statistics::TotalPayloadUpload() const { return m_total_upload_payload; }
t_uint64 Statistics::TotalPayloadDownload() const { return m_total_download_payload; }

t_uint64 Statistics::TotalProtocolUpload() const { return m_total_upload_protocol; }
t_uint64 Statistics::TotalProtocolDownload() const { return m_total_download_protocol; }



void Statistics::SecondTick()
{
		for (int i = history - 2; i >= 0; --i)
		{
				m_download_rate_history[i + 1] = m_download_rate_history[i];
				m_upload_rate_history[i + 1] = m_upload_rate_history[i];
				m_download_payload_rate_history[i + 1] = m_download_payload_rate_history[i];
				m_upload_payload_rate_history[i + 1] = m_upload_payload_rate_history[i];
		}

		m_download_rate_history[0] = m_downloaded_payload + m_downloaded_protocol;
		m_upload_rate_history[0] = m_uploaded_payload + m_uploaded_protocol;
		m_download_payload_rate_history[0] = m_downloaded_payload;
		m_upload_payload_rate_history[0] = m_uploaded_payload;

		m_downloaded_payload = 0;
		m_uploaded_payload = 0;
		m_downloaded_protocol = 0;
		m_uploaded_protocol = 0;

		m_mean_download_rate = 0;
		m_mean_upload_rate = 0;
		m_mean_download_payload_rate = 0;
		m_mean_upload_payload_rate = 0;

		for (int i = 0; i < history; ++i)
		{
				m_mean_download_rate += m_download_rate_history[i];
				m_mean_upload_rate += m_upload_rate_history[i];
				m_mean_download_payload_rate += m_download_payload_rate_history[i];
				m_mean_upload_payload_rate += m_upload_payload_rate_history[i];
		}

		m_mean_download_rate /= history;
		m_mean_upload_rate /= history;
		m_mean_download_payload_rate /= history;
		m_mean_upload_payload_rate /= history;
}



















}