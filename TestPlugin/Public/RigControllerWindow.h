#pragma once
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SExpanderArrow.h"
//#include "Widgets/SCanvas.h"
#include "Widgets/Colors/SColorBlock.h"
//#include "SCurveSlate.h"
#include "SCurvePanel.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboButton.h"
//#include "Widgets/Docking/SDockTabStack.h"
#include "Widgets/Docking/SDockTab.h"
//#include "Runtime/Slate/Private/Framework/Docking/SDockingTabStack.h"
#include "Widgets/Docking/SDockableTab.h"
#include "Widgets/Docking/SDockTabStack.h"
class RigControllerWindow :public SCompoundWidget
{
public:
	RigControllerWindow();

	SLATE_BEGIN_ARGS(RigControllerWindow)
	{

	}
	SLATE_END_ARGS()

	void Construct(const FArguments& args);

private:
	//TArray<TSharedRef<SCurveSlate>> curveList;
	TSharedRef<SDockTab> createFileTab(const FSpawnTabArgs& args);
	FReply browseBtnClicked();
	FReply importBtnClicked();


private:
	TSharedRef<SCurvePanel> curvePanel = SNew(SCurvePanel);
	TSharedRef<SEditableTextBox> curvePathEditText = SNew(SEditableTextBox).MinDesiredWidth(200.f);
	TSharedRef<SComboButton> comboBtn = SNew(SComboButton);


};


