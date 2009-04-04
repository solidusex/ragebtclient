
#pragma once
#include "Foundation.h"

enum MineState
{
		Covered = 0x0001,
		UnCovered = 0x0002,
		MarkFlag = 0x0003,
		MarkQuestion = 0x0004//,
		//Explode		= 0x0005
};

struct Mine
{
		size_t			num;
		bool			is_mine;
		MineState		state;
};

typedef std::pair<size_t,size_t>		PosT;
typedef std::list<PosT>					PosList;
typedef std::vector<PosT>				PosArray;

class MineSet : NonCopyable
{
private:
		typedef Array2D<Mine>  SetType;
		typedef Array2D<t_uint8>  ZeroMarkSet;
private:
		SetType					m_set;
		mutable ZeroMarkSet		m_zero_mark;
private:
		PosArray get_around(size_t row, size_t col)const
		{
				PosArray res;
				
				if(row == 0 && col == 0)
				{
						res.push_back(std::make_pair(row, col + 1));
						res.push_back(std::make_pair(row + 1, col));
						res.push_back(std::make_pair(row + 1, col + 1));

				}else if(row == 0 && col == (m_set.Columns() - 1))
				{
						res.push_back(std::make_pair(row, col - 1));
						res.push_back(std::make_pair(row + 1, col));
						res.push_back(std::make_pair(row + 1, col - 1));

				}else if(row == 0 && col != (m_set.Columns() - 1))
				{
						res.push_back(std::make_pair(row, col + 1));
						res.push_back(std::make_pair(row, col - 1));
						res.push_back(std::make_pair(row + 1, col));
						res.push_back(std::make_pair(row + 1, col - 1));
						res.push_back(std::make_pair(row + 1, col + 1));
						
				}else if(row == (m_set.Rows() - 1) && col == 0)
				{
						res.push_back(std::make_pair(row, col + 1));
						res.push_back(std::make_pair(row + 1, col));
						res.push_back(std::make_pair(row + 1, col + 1));
						
				}else if(row == (m_set.Rows() - 1) && col == (m_set.Columns() - 1))
				{
						res.push_back(std::make_pair(row, col - 1));
						res.push_back(std::make_pair(row - 1, col));
						res.push_back(std::make_pair(row - 1, col - 1));
				}else if(row == (m_set.Rows() - 1) && col != (m_set.Columns() - 1))
				{
						res.push_back(std::make_pair(row, col + 1));
						res.push_back(std::make_pair(row, col - 1));
						res.push_back(std::make_pair(row - 1, col));
						res.push_back(std::make_pair(row - 1, col - 1));
						res.push_back(std::make_pair(row - 1, col + 1));
				
				}else if(col == 0 && row != 0 && row != (m_set.Rows() - 1))
				{
						res.push_back(std::make_pair(row - 1 , col));
						res.push_back(std::make_pair(row + 1 , col));
						res.push_back(std::make_pair(row, col + 1));	
						res.push_back(std::make_pair(row - 1, col + 1));
						res.push_back(std::make_pair(row + 1, col + 1));

				}else if(col == (m_set.Columns() - 1) && row != 0 && row != (m_set.Rows() - 1))
				{
						res.push_back(std::make_pair(row - 1 , col));
						res.push_back(std::make_pair(row + 1 , col));
						res.push_back(std::make_pair(row, col - 1));	
						res.push_back(std::make_pair(row - 1, col - 1));
						res.push_back(std::make_pair(row + 1, col - 1));
				}else
				{
						res.push_back(std::make_pair(row - 1 , col));
						res.push_back(std::make_pair(row + 1 , col));
						res.push_back(std::make_pair(row, col - 1));	
						res.push_back(std::make_pair(row, col + 1));

						res.push_back(std::make_pair(row - 1, col - 1));
						res.push_back(std::make_pair(row - 1, col + 1));
						res.push_back(std::make_pair(row + 1, col - 1));
						res.push_back(std::make_pair(row + 1, col + 1));
				}

				return res;

		}


		void increase_mine_count(size_t row, size_t col)
		{
				PosArray pos_arr = get_around(row, col);
				
				for(size_t i = 0; i < pos_arr.size(); ++i)
				{
						m_set.Value(pos_arr[i].first, pos_arr[i].second).num++;
				}
		}

		void clear_zero_mark()const
		{
				for(size_t row = 0; row < m_zero_mark.Rows(); ++row)
				{
						for(size_t col = 0; col < m_zero_mark.Columns(); ++col)
						{
								m_zero_mark.Value(row, col) = 0;
						}

				}

		}

		void find_zero(size_t row, size_t col, PosArray &pos_arr)const
		{
				assert(m_set.Value(row, col).num == 0);
				m_zero_mark.Value(row, col) = 1;
				pos_arr.push_back(std::make_pair(row,col));
				
				PosArray around = get_around(row, col);
				for(size_t i = 0; i < around.size(); ++i)
				{
						const Mine &ref = m_set.Value(around[i].first,around[i].second);

						if(ref.state != MarkFlag && ref.num == 0 && m_zero_mark.Value(around[i].first,around[i].second) == 0)
						{
								find_zero(around[i].first,around[i].second, pos_arr);
						}

				}
		}
public:
		MineSet(size_t rows, size_t cols) : m_set(rows, cols), m_zero_mark(rows,cols)
		{
				
		}

		~MineSet()
		{


		}
		
		size_t GetRows()const
		{
				return m_set.Rows();
		}

		size_t GetColumns()const
		{
				return m_set.Columns();
		}

public:
		void SetPosAroundVisible(size_t row, size_t col)
		{
				PosArray around = get_around(row, col);

				for(size_t i = 0; i < around.size(); ++i)
				{
						Mine &ref = m_set.Value(around[i].first, around[i].second);
						if(!ref.is_mine && ref.state != MarkFlag)
						{
								ref.state = UnCovered;
						}
				}

		}

		PosArray  FindSeqZeroList(size_t row, size_t col)const
		{
				PosArray res;
				
				if(m_set.Value(row, col).num != 0)
				{
						//assert(0);
				}else
				{
						
						clear_zero_mark();
						find_zero(row, col,res);
				}
				return res;
		}

		const Mine& GetMine(size_t row, size_t col)const
		{
				return m_set.Value(row, col);
		}

		Mine& GetMine(size_t row, size_t col)
		{
				return m_set.Value(row, col);
		}
		
		
		void Init()
		{
				for(size_t row = 0; row < m_set.Rows(); ++row)
				{
						for(size_t col = 0; col < m_set.Columns(); ++col)
						{
								m_set.Value(row, col).state = Covered;
								if((rand() % 6) == 0)
								{
										m_set.Value(row, col).num = 0;
										m_set.Value(row, col).is_mine = true;
								}

						}

				}

				for(size_t row = 0; row < m_set.Rows(); ++row)
				{
						for(size_t col = 0; col < m_set.Columns(); ++col)
						{
								const Mine &ref_mine = m_set.Value(row, col);
								
								if(ref_mine.is_mine)
								{
										increase_mine_count(row, col);
								}

						}

				}

				for(size_t row = 0; row < m_set.Rows(); ++row)
				{
						for(size_t col = 0; col < m_set.Columns(); ++col)
						{
								Mine &ref_mine = m_set.Value(row, col);
								
								if(ref_mine.is_mine)
								{
										ref_mine.num = 0;
								}
						}
				}
		}

		
};

#if(0)
int main()
{
		
		MineSet mine_set(12,10);


		mine_set.Init();

		
		size_t r,c;

		
		while(true)
		{
				
				::system("cls");
				mine_set.Print();

				cin >> r >> c;

				PosList posl = mine_set.FindSeqZeroList(r,c);

				cout << "find zero nodes == " << posl.size() << endl;

				cin.get();
				cin.get();
		}

	
		

		



		
		printf("done\n");

		cin.get();
		
		return 0;
}


#endif


#if(0)
#if(0)
/*
		void find_zero(size_t row, size_t col, PosList &pos_list)const
		{
				assert(m_set.Value(row, col).num == 0);
				
				m_zero_mark.Value(row, col) = 1;
				pos_list.push_back(std::make_pair(row,col));

				if(row > 0 && row < (m_set.Rows() - 1) && col > 0 && col < (m_set.Columns() - 1))
				{
						for(size_t r = row - 1; r <= row + 1; ++r)
						{
								for(size_t c = col - 1; c <= col + 1; ++c)
								{
										if(r == row && c == col)continue;
										const Mine &ref = m_set.Value(r,c);
										if(ref.state != MarkFlag && ref.num == 0 && m_zero_mark.Value(r,c) == 0)
										{
												find_zero(r,c, pos_list);
										}
								}
						}
				}

				if(row == 0 && col > 0 && col != (m_set.Columns() - 1))
				{
						for(size_t r = row; r <= row+1; ++r)
						{
								for(size_t c = col - 1; c <= col + 1; ++c)
								{
										if(r == row && c == col)continue;

										const Mine &ref = m_set.Value(r,c);
										if(ref.state != MarkFlag && ref.num == 0 && m_zero_mark.Value(r,c) == 0)
										{
												find_zero(r,c, pos_list);
										}
								}
						}
				}


				if(row == (m_set.Rows() - 1) && col > 0 && col < (m_set.Columns() - 1))
				{
						for(size_t r = row - 1; r <= row; ++r)
						{
								for(size_t c = col - 1; c <= col + 1; ++c)
								{
										if(r == row && c == col)continue;

										const Mine &ref = m_set.Value(r,c);
										if(ref.state != MarkFlag && ref.num == 0 && m_zero_mark.Value(r,c) == 0)
										{
												find_zero(r,c, pos_list);
										}
								}
						}
				}

				if(row < (m_set.Rows() - 1) && row > 0 && col == 0)
				{
						for(size_t r = row - 1; r <= row + 1; ++r)
						{
								for(size_t c = col; c <= col + 1; ++c)
								{
										if(r == row && c == col)continue;

										const Mine &ref = m_set.Value(r,c);
										if(ref.state != MarkFlag && ref.num == 0 && m_zero_mark.Value(r,c) == 0)
										{
												find_zero(r,c, pos_list);
										}
								}
						}
				}

				if(row < (m_set.Rows() - 1) && row > 0 && col == (m_set.Columns() - 1))
				{
						for(size_t r = row - 1; r <= row + 1; ++r)
						{
								for(size_t c = col - 1; c <= col; ++c)
								{
										if(r == row && c == col)continue;

										const Mine &ref = m_set.Value(r,c);
										if(ref.state != MarkFlag && ref.num == 0 && m_zero_mark.Value(r,c) == 0)
										{
												find_zero(r,c, pos_list);
										}
								}
						}
				}

				if(row == 0 && col == 0)
				{

						for(size_t r = row; r <= row + 1; ++r)
						{
								for(size_t c = col; c <= col + 1; ++c)
								{
										if(r == row && c == col)continue;

										const Mine &ref = m_set.Value(r,c);
										if(ref.state != MarkFlag && ref.num == 0 && m_zero_mark.Value(r,c) == 0)
										{
												find_zero(r,c, pos_list);
										}
								}
						}



				}

				if(row == (m_set.Rows() - 1) && col == 0)
				{

						for(size_t r = row - 1; r <= row; ++r)
						{
								for(size_t c = col; c <= col + 1; ++c)
								{
										if(r == row && c == col)continue;

										const Mine &ref = m_set.Value(r,c);
										if(ref.state != MarkFlag && ref.num == 0 && m_zero_mark.Value(r,c) == 0)
										{
												find_zero(r,c, pos_list);
										}
								}
						}
				}


				if(row == 0 && col == (m_set.Columns() - 1))
				{
						for(size_t r = row; r <= row + 1; ++r)
						{
								for(size_t c = col - 1; c <= col; ++c)
								{
										if(r == row && c == col)continue;

										const Mine &ref = m_set.Value(r,c);
										if(ref.state != MarkFlag && ref.num == 0 && m_zero_mark.Value(r,c) == 0)
										{
												find_zero(r,c, pos_list);
										}
								}
						}

				}


				if(row == (m_set.Rows() - 1) && col == (m_set.Columns() - 1))
				{
						for(size_t r = row - 1; r <= row; ++r)
						{
								for(size_t c = col - 1; c <= col; ++c)
								{
										if(r == row && c == col)continue;

										const Mine &ref = m_set.Value(r,c);
										if(ref.state != MarkFlag && ref.num == 0 && m_zero_mark.Value(r,c) == 0)
										{
												find_zero(r,c, pos_list);
										}
								}
						}

				}
		}*/

		void increase_mine_count(size_t row, size_t col)
		{
				if(row == 0 && col == 0)
				{
						get_mine_by_pos(row, col, RIGHT).num++;
						get_mine_by_pos(row, col, DOWN).num++;
						get_mine_by_pos(row, col, DOWN_RIGHT).num++;
				}else if(row == 0 && col == (m_set.Columns() - 1))
				{
						get_mine_by_pos(row, col, LEFT).num++;
						get_mine_by_pos(row, col, DOWN).num++;
						get_mine_by_pos(row, col, DOWN_LEFT).num++;
				}else if(row == 0 && col != (m_set.Columns() - 1))
				{
						get_mine_by_pos(row, col, RIGHT).num++;
						get_mine_by_pos(row, col, LEFT).num++;
						get_mine_by_pos(row, col, DOWN).num++;
						get_mine_by_pos(row, col, DOWN_LEFT).num++;
						get_mine_by_pos(row, col, DOWN_RIGHT).num++;
						
				}else if(row == (m_set.Rows() - 1) && col == 0)
				{
						get_mine_by_pos(row, col, RIGHT).num++;
						get_mine_by_pos(row, col, UP).num++;
						get_mine_by_pos(row, col, UP_RIGHT).num++;
						
				}else if(row == (m_set.Rows() - 1) && col == (m_set.Columns() - 1))
				{
						get_mine_by_pos(row, col, LEFT).num++;
						get_mine_by_pos(row, col, UP).num++;
						get_mine_by_pos(row, col, UP_LEFT).num++;
				}else if(row == (m_set.Rows() - 1) && col != (m_set.Columns() - 1))
				{
						get_mine_by_pos(row, col, RIGHT).num++;
						get_mine_by_pos(row, col, LEFT).num++;
						get_mine_by_pos(row, col, UP).num++;
						get_mine_by_pos(row, col, UP_LEFT).num++;
						get_mine_by_pos(row, col, UP_RIGHT).num++;
				}else if(col == 0 && row != 0 && row != (m_set.Rows() - 1))
				{
						get_mine_by_pos(row, col, UP).num++;
						get_mine_by_pos(row, col, DOWN).num++;
						get_mine_by_pos(row, col, RIGHT).num++;
						get_mine_by_pos(row, col, UP_RIGHT).num++;
						get_mine_by_pos(row, col, DOWN_RIGHT).num++;
				}else if(col == (m_set.Columns() - 1) && row != 0 && row != (m_set.Rows() - 1))
				{
						get_mine_by_pos(row, col, UP).num++;
						get_mine_by_pos(row, col, DOWN).num++;
						get_mine_by_pos(row, col, LEFT).num++;
						get_mine_by_pos(row, col, UP_LEFT).num++;
						get_mine_by_pos(row, col, DOWN_LEFT).num++;
				}else
				{
						get_mine_by_pos(row, col, UP).num++;
						get_mine_by_pos(row, col, DOWN).num++;
						get_mine_by_pos(row, col, LEFT).num++;
						get_mine_by_pos(row, col, RIGHT).num++;
						get_mine_by_pos(row, col, UP_LEFT).num++;
						get_mine_by_pos(row, col, UP_RIGHT).num++;
						get_mine_by_pos(row, col, DOWN_LEFT).num++;
						get_mine_by_pos(row, col, DOWN_RIGHT).num++;
				}
		}

		enum Pos
		{
			UP = 0x0001,
			DOWN = 0x0002,
			LEFT = 0x0003,
			RIGHT = 0x0004,
			UP_LEFT = 0x0005,
			UP_RIGHT = 0x0006,
			DOWN_LEFT = 0x0007,
			DOWN_RIGHT = 0x0008
		};

				inline Mine& get_mine_by_pos(size_t row, size_t col, Pos pos)
		{
				switch(pos)
				{
				case UP:
						return m_set.Value(row -1, col);
				case DOWN:
						return m_set.Value(row + 1, col);
				case LEFT:
						return m_set.Value(row, col -1);
				case RIGHT:
						return m_set.Value(row, col + 1);
				case UP_LEFT:
						return m_set.Value(row -1, col -1);
				case UP_RIGHT:
						return m_set.Value(row -1, col + 1);
				case DOWN_LEFT:
						return m_set.Value(row + 1, col -1);
				case DOWN_RIGHT:
						return m_set.Value(row + 1, col +1);
				default:
						assert(0);
				}
		}

#endif
#endif