// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SLeafWidget.h"
#include "Widgets/SCanvas.h"

#include "Json.h"
class FPaintArgs;
class FSlateWindowElementList;


class TESTPLUGIN_API SCurveSlate : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SCurveSlate)
		: _Color(FColor(120, 120, 120, 255))

		, _OnMouseButtonDown()
		,_OnMouseMove()
		, _Size(FVector2D(16, 16))


	{}

	SLATE_ATTRIBUTE(FLinearColor, Color)


	SLATE_EVENT(FPointerEventHandler, OnMouseButtonDown)


	SLATE_EVENT(FPointerEventHandler, OnMouseMove)


	SLATE_ATTRIBUTE(FVector2D, Size)


	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);

	void setCanvasSlot( SCanvas::FSlot*);

	void setPointArray(TArray<FVector2D>,bool,FString);


public:
	SCanvas::FSlot* canvasSlot;
	bool isHovered;
	bool isSelected;

private:



	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	virtual FVector2D ComputeDesiredSize(float) const override;

	//bool isStatic;
	FString curveName;
	FColor normalColor;
	FColor hoverColor;
	FColor selectColor;

private:

	TAttribute< FLinearColor > Color;
	TArray<FVector2D> PointArray;
	TAttribute<FVector2D> ColorBlockSize;
	int lyId;
};
