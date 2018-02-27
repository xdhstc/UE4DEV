// Fill out your copyright notice in the Description page of Project Settings.

#include "SCurveSlate.h"
#include "SlateOptMacros.h"
#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCurveSlate::Construct(const FArguments& InArgs)
{
	Color = InArgs._Color;
	ColorBlockSize = InArgs._Size;
	normalColor = FColor(120, 120, 120, 255);
	hoverColor = FColor(0, 0, 200, 255);
	selectColor = FColor(0, 200, 0, 255);
	isHovered = false;
	isSelected = false;
	//isStatic = false;
	lyId = 9;

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCurveSlate::setCanvasSlot( SCanvas::FSlot* _inSlot)
{
	canvasSlot = _inSlot;
}

void SCurveSlate::setPointArray(TArray<FVector2D> _pointArray, bool _isStatic, FString _curveName)
{
	//isStatic = _isStatic;
	curveName = _curveName;
	PointArray = _pointArray;
	//PointArray.Pop();
	SetToolTipText(FText::FromString(curveName));
	lyId = 9;
}







int32 SCurveSlate::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FSlateBrush* GenericBrush = FCoreStyle::Get().GetBrush("GenericWhiteBox");
	const ESlateDrawEffect DrawEffects = ESlateDrawEffect::None;
	FLinearColor InColor = Color.Get();
	
	//FSlateDrawElement::MakeLines(OutDrawElements, LayerId + 1, AllottedGeometry.ToPaintGeometry(), PointArray, DrawEffects, InWidgetStyle.GetColorAndOpacityTint()*InColor, true, 2.0f);
	FSlateDrawElement::MakeLines(OutDrawElements, lyId, AllottedGeometry.ToPaintGeometry(), PointArray, DrawEffects, InWidgetStyle.GetColorAndOpacityTint()*InColor, false, 1.0f);

	return lyId;
}

void SCurveSlate::OnMouseEnter(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{

	Color = hoverColor;
	isHovered = true;
	UE_LOG(LogTemp, Warning, TEXT("curve enter"));


	


}

void SCurveSlate::OnMouseLeave(const FPointerEvent & MouseEvent)
{
	if (!isSelected)
	{
		Color = normalColor;
		isHovered = false;
	}

	UE_LOG(LogTemp, Warning, TEXT("curve leave"));


}

FVector2D SCurveSlate::ComputeDesiredSize(float) const
{
	return ColorBlockSize.Get();
}






