#pragma once
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SExpanderArrow.h"
//#include "Widgets/SCanvas.h"
#include "Widgets/Colors/SColorBlock.h"
//#include "SCurveSlateStatic.h"
#include "SCurveSlate.h"
enum WindowState
{
	Normal,
	CanvasMove,
	CurveMove,

};

//this curvedata struct is before scale and offset
struct CurveData
{
	FString name;
	TArray<FVector2D> pointArray;
	FVector2D vertexOffsetVector;
	FVector2D curveOffsetVector;
	FVector2D size;
	bool isStatic;
	bool xLocked;
	bool yLocked;
	bool zLocked;
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	float zMin;
	float zMax;
	TSharedRef<SCurveSlate>* curve;

	//TSharedRef<SCurveSlate> curve;
};





class SCurvePanel :public SCompoundWidget
{
public:
	SCurvePanel();

	SLATE_BEGIN_ARGS(SCurvePanel)
	{

	}
	SLATE_END_ARGS()

	void Construct(const FArguments& args);
	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool SupportsKeyboardFocus() const override { return true; }

	
	

	//void OnCurveEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	//void OnCurveEnter1(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, FString&);
	//FReply clickTest(FString myS);


	FReply testBtnClicked();
	void loadJson(TMap<FString, TSharedPtr<FJsonValue>>);
	void clearPanel();

public:
	//TSharedRef<SCanvas> ctlCanvas = SNew(SCanvas);
	TSharedRef<SCanvas> ctlCanvas = SNew(SCanvas);

	//TSharedRef<STextBlock> testTxt = SNew(STextBlock);
	//SCanvas::FSlot *testTxtSlot;

	TSharedRef<SCurveSlate> testCurve = SNew(SCurveSlate);
	SCanvas::FSlot *testCurveSlot;

	TSharedRef<SCurveSlate> testCurve1 = SNew(SCurveSlate);
	SCanvas::FSlot *testCurveSlot1;


	TSharedRef<SButton> testBtn = SNew(SButton).Text(FText::FromString("sdf"));
	SCanvas::FSlot *testBtnSlot;

	FVector2D clickPos;
	FVector2D selectCurvePos;
	FVector2D canvasPos;
	FVector2D txtStartPos;
	FVector2D canvasOffset;
	//FSlateRenderTransform canvasTrans;
	//bool textSelected;
	WindowState winState;
	//SCurveSlate* selectCurve;
	float drawCanvasScale;
	float canvasScale;
	float panelMaxWidth;
	float panelMaxHeight;
	TSharedRef<SCurveSlate>* selectCurve;

private:
	TArray<TSharedRef<SCurveSlate>> curveList;
	TArray<CurveData> curveDataArray;

private:
	void initParam();
	//void initCanvas();
	void setCanvasTrans();
	//void offsetCanvas(FVector2D);
	//void scaleCanvas(FScale2D);


	void myDebug(FString);

};


