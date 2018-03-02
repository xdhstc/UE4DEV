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
	staticColor = FColor(120, 120, 120, 255);
	dynamicColor = FColor(0, 0, 200, 255);
	hoverColor = FColor(67, 255, 163);
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
	contorllOffsetVector = FVector2D(0, 0);
}

void SCurveSlate::setPointArray(TArray<FVector2D> _pointArray, bool _isStatic, FString _curveName)
{
	//isStatic = _isStatic;
	curveName = _curveName;
	TArray<FString> stringArray;
	curveName.ParseIntoArray(stringArray, TEXT("|"));
	PointArray = _pointArray;
	if (_isStatic)
	{
		normalColor = staticColor;
	}
	else
	{
		normalColor = dynamicColor;
	}
	Color = normalColor;
	//PointArray.Pop();
	SetToolTipText(FText::FromString(stringArray.Last()));
	lyId = 9;
}

void SCurveSlate::setMoveAttr(bool _xLocked, bool _yLocked, bool _zLocked, float _xMin, float _xMax, float _yMin, float _yMax, float _zMin, float _zMax, FVector2D _curveOffsetVector)
{
	xLocked = _xLocked;
	yLocked = _yLocked;
	zLocked = _zLocked;
	xMin = _xMin;
	xMax = _xMax;
	yMin = -_yMax;
	yMax = -_yMin;
	zMin = _zMin;
	zMax = _zMax;
	curveOffsetVector = _curveOffsetVector;
	realCtlOffset = FVector2D(0.f, 0.f);
	setCurveTrans();




}

void SCurveSlate::setCurveTrans()
{
	SetRenderTransform(FSlateRenderTransform(curveOffsetVector));
}

void SCurveSlate::setCurveActive(bool _isActive)
{
	if (_isActive)
	{
		Color = hoverColor;
		isHovered = true;
	}
	else
	{
		if (!isSelected)
		{
			Color = normalColor;
			isHovered = false;
		}

	}
}

void SCurveSlate::moveCurveStart(FVector2D _canvasBeforeVec, FVector2D _mouseDownVec, FVector2D _mouseCurrentVec, float _canvasScale, float _drawCanvasScale)
{

	compareVec = contorllOffsetVector+(_mouseCurrentVec - _mouseDownVec) / (_canvasScale*_drawCanvasScale);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(compareVec.Y));



	if (xLocked && yLocked)
	{
		curveOffsetVector = FVector2D(0, 0);
	}

	if (xLocked && !yLocked)
	{
		if (compareVec.Y < yMin)
		{
			curveOffsetVector.Y = _canvasBeforeVec.Y + (yMin - contorllOffsetVector.Y)*_drawCanvasScale;
		}

		if (compareVec.Y >= yMin && compareVec.Y <= yMax)
		{
			curveOffsetVector.Y = _canvasBeforeVec.Y + (_mouseCurrentVec.Y - _mouseDownVec.Y) / _canvasScale;
		}

		if (compareVec.Y > yMax)
		{
			curveOffsetVector.Y = _canvasBeforeVec.Y + (yMax - contorllOffsetVector.Y)*_drawCanvasScale;
		}

		realCtlOffset = curveOffsetVector / _drawCanvasScale;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(-realCtlOffset.Y));
	}


	if (!xLocked && yLocked)
	{
		if (compareVec.X < xMin)
		{
			curveOffsetVector.X = _canvasBeforeVec.X + (xMin - contorllOffsetVector.X)*_drawCanvasScale;
		}

		if (compareVec.X >= xMin && compareVec.X <= xMax)
		{
			curveOffsetVector.X = _canvasBeforeVec.X + (_mouseCurrentVec.X - _mouseDownVec.X) / _canvasScale;
		}

		if (compareVec.X > xMax)
		{
			curveOffsetVector.X = _canvasBeforeVec.X + (xMax - contorllOffsetVector.X)*_drawCanvasScale;
		}

		realCtlOffset = curveOffsetVector / _drawCanvasScale;


		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(realCtlOffset.X));




	}


	if (!xLocked && !yLocked)
	{
		if (compareVec.X < xMin)
		{
			curveOffsetVector.X = _canvasBeforeVec.X + (xMin - contorllOffsetVector.X)*_drawCanvasScale;
		}

		if (compareVec.X >= xMin && compareVec.X <= xMax)
		{
			curveOffsetVector.X = _canvasBeforeVec.X + (_mouseCurrentVec.X - _mouseDownVec.X) / _canvasScale;
		}

		if (compareVec.X > xMax)
		{
			curveOffsetVector.X = _canvasBeforeVec.X + (xMax - contorllOffsetVector.X)*_drawCanvasScale;
		}

		



		if (compareVec.Y < yMin)
		{
			curveOffsetVector.Y = _canvasBeforeVec.Y + (yMin - contorllOffsetVector.Y)*_drawCanvasScale;
		}

		if (compareVec.Y >= yMin && compareVec.Y <= yMax)
		{
			curveOffsetVector.Y = _canvasBeforeVec.Y + (_mouseCurrentVec.Y - _mouseDownVec.Y) / _canvasScale;
		}

		if (compareVec.Y > yMax)
		{
			curveOffsetVector.Y = _canvasBeforeVec.Y + (yMax - contorllOffsetVector.Y)*_drawCanvasScale;
		}

		realCtlOffset = curveOffsetVector / _drawCanvasScale;


		UE_LOG(LogTemp, Warning, TEXT("%s"), *realCtlOffset.ToString());

















	}







	//curveOffsetVector = _canvasBeforeVec + (_mouseCurrentVec - _mouseDownVec) / _canvasScale;
	setCurveTrans();
	//selectCurve->Get().SetRenderTransform(FSlateRenderTransform(selectCurvePos+(mouseEvent.GetScreenSpacePosition() - clickPos)/canvasScale));


}

void SCurveSlate::moveCuveEnd()
{
	contorllOffsetVector = realCtlOffset;
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
	setCurveActive(true);
}


void SCurveSlate::OnMouseLeave(const FPointerEvent & MouseEvent)
{
	setCurveActive(false);
	UE_LOG(LogTemp, Warning, TEXT("curve leave"));


}

FVector2D SCurveSlate::ComputeDesiredSize(float) const
{
	return ColorBlockSize.Get();
}






