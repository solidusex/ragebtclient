#include "StdAfx.h"
#include "SplitterWndEx.h"



CSplitterWndEx::CSplitterWndEx(void)
{


}







CSplitterWndEx::~CSplitterWndEx(void)
{
		

}





BOOL CSplitterWndEx::ReplaceView(int row, int col,CRuntimeClass * pViewClass,SIZE size)
{
		CCreateContext context;
		BOOL bSetActive;


		//if ((GetPane(row,col)->IsKindOf(pViewClass))==TRUE)
		//		return FALSE;

		

		// Get pointer to CDocument object so that it can be used in the creation 
		// process of the new view
		CDocument * pDoc= ((CView *)GetPane(row,col))->GetDocument();
		CView * pActiveView = GetParentFrame()->GetActiveView();
		if (pActiveView==NULL || pActiveView==GetPane(row,col))
				bSetActive=TRUE;
		else
				bSetActive=FALSE;
#if(0)
		// set flag so that document will not be deleted when view is destroyed
		pDoc->m_bAutoDelete=FALSE;    
		// Delete existing view 
		((CView *) GetPane(row,col))->DestroyWindow();
		// set flag back to default 
		pDoc->m_bAutoDelete=TRUE;
#endif
		
		pDoc->RemoveView((CView*)GetPane(row, col));
		((CView *) GetPane(row,col))->DestroyWindow();

		// Create new view                      
#if(0)
		context.m_pNewViewClass=pViewClass;
		context.m_pCurrentDoc=pDoc;
		context.m_pNewDocTemplate = NULL;
		context.m_pLastView=NULL;
		context.m_pCurrentFrame=NULL;
#endif

		context.m_pNewViewClass=pViewClass;
		context.m_pCurrentDoc=pDoc;
		context.m_pNewDocTemplate = pDoc->GetDocTemplate();
		context.m_pLastView=NULL;
		context.m_pCurrentFrame=NULL;
		

		CreateView(row,col,pViewClass,size, &context);

		CView * pNewView= (CView *)GetPane(row,col);

		if (bSetActive==TRUE)
				GetParentFrame()->SetActiveView(pNewView);

		RecalcLayout(); 
		GetPane(row,col)->SendMessage(WM_PAINT);

		return TRUE;
}