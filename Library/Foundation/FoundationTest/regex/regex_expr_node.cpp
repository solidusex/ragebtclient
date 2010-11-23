
#include "regex_expr_node.h"



void	CharSet::Insert(const CharSet::Range &new_range)
{
		Range	range = new_range;
		for(RangeList::iterator it = m_list.begin(); it != m_list.end(); ++it)
		{
				Range &curr = *it;

				if(curr.beg < range.beg)
				{
						if(curr.end < range.beg)
						{
								/*
								[ curr ]
								[ range ]
								*/
						}else if(curr.end < range.end)
						{
								/*
								[ curr ]
								[ range ]
								*/
								range.beg = curr.end + 1;
						}else
						{
								/*
								[   curr    ]
								[ range1]
								[ range2]
								*/

								return;
						}

				}else if(curr.beg == range.beg)
				{
						if(curr.end < range.end)
						{
								/*
								[curr]
								[ range ]
								*/

								range.beg = curr.end + 1;

						}else if(curr.end == range.end)
						{
								/*
								[curr ]
								[range]
								*/

								return;
						}else
						{
								/*
								[curr ]
								[  range  ]

								*/

								range.beg = curr.end + 1;
						}

				}else /*if(curr.beg > curr.end)*/
				{
						if(curr.end < range.end)
						{
								/*
								[curr]
								[ range  ]
								*/
								curr.beg = range.beg;
								range.beg = curr.end + 1;


						}else if(curr.end == range.end)
						{
								/*
								[curr]
								[ range  ]
								*/

								curr.beg = range.beg;
								return;
						}else
						{
								/*
								[curr]
								[range]
								*/

								curr.beg = range.beg;
								return;
						}
				}
		}

		m_list.push_back(range);
}

