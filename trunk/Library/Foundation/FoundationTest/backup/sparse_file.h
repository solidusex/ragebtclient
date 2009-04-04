#if(0)
static const t_uint64 MIN_BLOCK_LEN = 16*KB; /*如果剩余的数据块小于MIN_BLOCK_LEN就不再分割了*/
static const t_uint64 MIN_INC_SIZE  = 512 *KB;

static const t_byte FREE = 0x00;
static const t_byte USED = 0x01;


/*每个piece info为len + 一个字节的SPACE_TYPE + len,len不可为0*/
typedef struct SparseFLHeader
{
		t_uint64		data_len;//整个数据段长度，整个文件长度应该等于20(hash) + 8(data_len) + data_len
		t_byte			hash[20];//20字节的数据段sha1 hash value，如果数据段为0，则hash值为0
}SparseFLHeader;

static const t_uint64 BASE_START_POS = 8 + 20;

typedef struct SparseBlockHeader
{
		t_uint64		prev_pos;/*前一块的地址，如果前一块为0，则代表本块为第一块*/
		t_uint64		block_len;/*整个数据段长度，整个文件长度应该等于data_len + 20(hahs) + 4(data_len) + 1(header type)*/
		t_byte			in_use;
}SparseBlockHeader;

static const t_uint64 BLOCK_HEADER_LEN = 17;

class SparseBlockData : private NonCopyable
{
private:
		t_uint64				m_start_pos;/*data起始地址，不算header*/
		t_uint64				m_len;		/*本块数据块长度（不包括header长度）*/
		FileSysSpace::File		*m_file;
public:
		SparseBlockData(FileSysSpace::File *pfile, t_uint64 start_pos, t_uint64 len) 
				: m_file(pfile)
				, m_start_pos(start_pos)
				, m_len(len)
		{
				assert(pfile != NULL && pfile->IsOpen());
				assert(m_start_pos > 0);
				assert(m_len > m_start_pos);
				assert(m_len > 17);
		}

		void ResetBlock(t_uint64 start_pos, t_uint64 len)
		{
				assert(m_start_pos > 0);
				assert(m_len > m_start_pos);
				assert(m_len > 17);
				
				m_start_pos = start_pos;
				m_len = len;
		}

		~SparseBlockData()
		{

		}
		
		t_uint64 GetDataLength()
		{
				return m_len;
		}

		bool Read(t_byte *pdata)
		{
				assert(m_file->IsOpen() && m_file->GetLength() >= (m_start_pos + m_len));

				m_file->Seek(m_start_pos, BEG, true);

				return (m_file->Read(pdata, m_len) == m_len);
		}

		bool Write(const t_byte *pdata)
		{
				assert(m_file->IsOpen() && m_file->GetLength() >= (m_start_pos + m_len));
				m_file->Seek(m_start_pos, BEG, true);
				return (m_file->Write(pdata, m_len) == m_len);
		}
};

#define HEADER_LEN (20 + 8)
class SparseFileList : private NonCopyable
{
private:
		FileSysSpace::File		m_file;
		SparseFLHeader			m_header;

private:
		void set_empty_sparse_file()
		{
				t_byte buf[20];
				assert(m_file.IsOpen());
				m_file.SetLength(HEADER_LEN);
				m_file.SeekToBeg();
				memset(buf, 0, HEADER_LEN);

				if(m_file.Write(buf, HEADER_LEN) != HEADER_LEN)
				{
						throw ExceptionSpace::FileException("file write failed\n");
				}
				m_header.data_len = 0;
				memset(m_header.hash, 0, 20);
		}
		
		void hash_data_len(t_byte val[20]);
public:
		SparseFileList(const std::wstring &path)
		{
				FileSysSpace::Path p(path);
				
				if(p.GetPathType() != PATH_FILE)throw ExceptionSpace::InvalidArgumentException("path name invalid : SparseFileList(const std::wstring &path\n");

				if(!p.Make())throw ExceptionSpace::IOException("CreateFile failed\n");

				if(!m_file.Open(path, F_READ_WRITE|F_OPEN_EXISTED|F_NOSHARED))throw ExceptionSpace::FileException("Open file filed : error code == %d\n", GetLastError());

				t_byte buf[HEADER_LEN];
				
				t_uint64 total_len = m_file.GetLength();
				if(total_len < HEADER_LEN)
				{
						printf("invalid header resize file\n");
						set_empty_sparse_file();
				}else
				{
						if(m_file.Read(buf, HEADER_LEN) != HEADER_LEN)
						{
								throw ExceptionSpace::FileException("file read failed\n");
						}

						
						m_header.data_len = ByteOrder::FromNetOrderToNativeOrder(*(t_uint64*)buf);
						if(m_header.data_len == 0)
						{
								printf("zero data\n");
								memset(m_header.hash, 0, 20);
						}else
						{
								if(((total_len - HEADER_LEN) != m_header.data_len) || (m_header.data_len < MIN_BLOCK_LEN))
								{
										printf("bad data_file\n");
										set_empty_sparse_file();
										return;
								}else
								{
										t_byte hash_val[20];
										hash_data_len(hash_val);

										if(memcmp(hash_val, m_header.hash, 20) != 0)
										{
												set_empty_sparse_file();
												return;
										}

								}
						}
				}
		}



		SparseBlockData* AllocateBlock(size_t data_len)
		{
				assert(data_len > 0 && m_file.IsOpen() && m_file.GetLength() >= HEADER_LEN);

				t_uint64 total_len = m_file.GetLength();
				t_uint64 alloc_len = 0;

				if(total_len == HEADER_LEN)
				{
						alloc_len = data_len > MIN_INC_SIZE ? data_len  : MIN_INC_SIZE;
						alloc_len += BLOCK_HEADER_LEN;
				}


		}


};
#endif
