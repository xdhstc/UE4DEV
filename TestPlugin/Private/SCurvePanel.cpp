
#include "SCurvePanel.h"

#include "TestPlugin.h"
//#include "Runtime/Engine/Classes/Engine/World.h"
#define LOCTEXT_NAMESPACE "RigControllerWindow"

SCurvePanel::SCurvePanel()
{
	initParam();
	setCanvasTrans();
}

void SCurvePanel::initParam()
{
	winState = WindowState::Normal;
	panelMaxWidth = 1500.f;
	panelMaxHeight = 980.f;
	canvasScale = 1.0f;
	canvasOffset = FVector2D(0.f, 0.f);
}

void SCurvePanel::setCanvasTrans()
{
	ctlCanvas->SetRenderTransform(FSlateRenderTransform(FScale2D(canvasScale, canvasScale), canvasOffset));
}

void SCurvePanel::clearPanel()
{
	for (int i = 0; i < curveList.Num(); i++)
	{
		ctlCanvas->RemoveSlot(curveList[i]);
	}
	curveList.Empty();
}

void SCurvePanel::Construct(const FArguments & args)
{
	ChildSlot
		[
			ctlCanvas
		];
	
	testCurveSlot1 = &ctlCanvas->AddSlot();
	testCurveSlot1->Position(FVector2D(350, 170));
	testCurveSlot1->Size(FVector2D(200.0f, 80.0f));
	testCurve1->setCanvasSlot(testCurveSlot1);

	TArray<FVector2D> vecList1;
	vecList1.Add(FVector2D(0, 0));
	vecList1.Add(FVector2D(30, 10));
	vecList1.Add(FVector2D(50, 10));
	vecList1.Add(FVector2D(0, 0));
	testCurve1->setPointArray(vecList1,false,FString("curve1"));

	testCurveSlot1->AttachWidget(testCurve1.Get().AsShared());
	curveList.Add(testCurve1);




	testCurveSlot = &ctlCanvas->AddSlot();
	testCurveSlot->Position(FVector2D(300, -40));
	testCurveSlot->Size(FVector2D(100.0f, 40.0f));
	testCurve->setCanvasSlot(testCurveSlot);
	//testCurve->SetToolTipText(LOCTEXT("bbbb", "curve"));

	TArray<FVector2D> vecList;
	vecList.Add(FVector2D(10, 10));
	vecList.Add(FVector2D(50, 40));
	vecList.Add(FVector2D(100, 10));
	vecList.Add(FVector2D(10, 10));


	testCurve->setPointArray(vecList, false, FString("curve"));
	testCurveSlot->AttachWidget(testCurve.Get().AsShared());

	curveList.Add(testCurve);

	



	testBtn->SetToolTipText(LOCTEXT("aaa", "4444444"));
	
	testBtn->SetOnClicked(FOnClicked::CreateRaw(this, &SCurvePanel::testBtnClicked));
	testBtnSlot = &ctlCanvas->AddSlot();
	testBtnSlot->HAlign(HAlign_Center);
	testBtnSlot->VAlign(VAlign_Center);
	testBtnSlot->Position(FVector2D(200, 40));
	testBtnSlot->Size(FVector2D(100.0f, 40.0f));
	testBtnSlot->AttachWidget(testBtn.Get().AsShared());

}


void SCurvePanel::loadJson(TMap<FString, TSharedPtr<FJsonValue>> _jsonMap)
{
	clearPanel();
	curveDataArray.Empty();
	initParam();
	setCanvasTrans();
	TArray<float> xAllArray;
	TArray<float> yAllArray;
	for (TMap<FString, TSharedPtr<FJsonValue>>::TIterator It(_jsonMap); It; ++It)
	{
		CurveData curveData;
		TSharedPtr< FJsonObject > contentPtr = It.Value()->AsObject();
		curveData.name = It.Key();
		curveData.isStatic = contentPtr->GetBoolField("static");
		TSharedPtr< FJsonObject > vertexDictPtr = contentPtr->GetObjectField(FString("vertex_dict"));
		TMap<FString, TSharedPtr<FJsonValue>> vertexDictMap = vertexDictPtr->Values;
		vertexDictMap.KeySort([](FString A, FString B) {return A < B; });
		TArray<float> curveXArray;
		TArray<float> curveYArray;
		for (TMap<FString, TSharedPtr<FJsonValue>>::TIterator Ik(vertexDictMap); Ik; ++Ik)
		{
			const  TArray<TSharedPtr<FJsonValue>> jsonVexList = Ik.Value()->AsArray();
			float tmpX = jsonVexList[0]->AsNumber();
			float tmpY = -jsonVexList[1]->AsNumber();
			xAllArray.Add(tmpX);
			yAllArray.Add(tmpY);
			curveXArray.Add(tmpX);
			curveYArray.Add(tmpY);
			curveData.pointArray.Add(FVector2D(tmpX, tmpY));
		}
		curveXArray.Sort([](float A, float B) {return A < B; });
		curveYArray.Sort([](float A, float B) {return A < B; });
		curveData.offsetVector = FVector2D(curveXArray[0], curveYArray[0]);
		curveData.size = FVector2D(curveXArray.Last() - curveXArray[0], curveYArray.Last() - curveYArray[0]);
		curveDataArray.Add(curveData);
	}

	xAllArray.Sort([](float A, float B) {return A < B; });
	yAllArray.Sort([](float A, float B) {return A < B; });

	float xMin = xAllArray[0];
	float xMax = xAllArray.Last();
	float yMin = yAllArray[0];
	float yMax = yAllArray.Last();

	float xScale = panelMaxWidth / (xMax - xMin);
	float yScale = panelMaxHeight / (yMax - yMin);
	float tmpCanvasScale = xScale < yScale ? xScale : yScale;

	FVector2D AllOffsetVector = FVector2D(-xMin, -yMin);

	for (int i = 0; i < curveDataArray.Num(); i++)
	{
		TArray<FVector2D> vexList;
		for (int j = 0; j < curveDataArray[i].pointArray.Num(); j++)
		{
			vexList.Add((curveDataArray[i].pointArray[j] - curveDataArray[i].offsetVector)*tmpCanvasScale);
		}


		SCanvas::FSlot *myCurveSlot = &ctlCanvas->AddSlot();
		myCurveSlot->Position((curveDataArray[i].offsetVector + AllOffsetVector)*tmpCanvasScale);
		if (!curveDataArray[i].isStatic)
		{
			myCurveSlot->Size(curveDataArray[i].size*tmpCanvasScale);
		}
		
		TSharedRef<SCurveSlate> myCurve = SNew(SCurveSlate);
		myCurve->setCanvasSlot(myCurveSlot);
		myCurve->setPointArray(vexList, curveDataArray[i].isStatic, curveDataArray[i].name);
		myCurve->SetRenderTransform(FSlateRenderTransform(FVector2D(0, 0)));
		myCurveSlot->AttachWidget(myCurve.Get().AsShared());
		curveList.Add(myCurve);
	}
}



FReply SCurvePanel::OnMouseWheel(const FGeometry & MyGeometry, const FPointerEvent & mouseEvent)
{
	
	const FVector2D WidgetSpaceCursorPos = MyGeometry.AbsoluteToLocal(mouseEvent.GetScreenSpacePosition());
	const int32 ZoomLevelDelta = FMath::FloorToInt(mouseEvent.GetWheelDelta());
	float tmpScale = mouseEvent.GetWheelDelta()/10.f;
	canvasScale = canvasScale *(1 + tmpScale);
	canvasOffset -= (WidgetSpaceCursorPos - canvasOffset)*tmpScale;
	//canvasScale = canvasScale > 0.1f ? canvasScale : 0.1f;
	//canvasScale = canvasScale < 5.0f ? canvasScale : 5.0f;
	setCanvasTrans();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(canvasScale));
	return FReply::Handled();
}

FReply SCurvePanel::OnMouseButtonDown(const FGeometry & MyGeometry, const FPointerEvent & mouseEvent)
{

	if (mouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("canvas left click"));
		{
			for (int i = 0; i < curveList.Num(); i++)
			{
				if (curveList[i]->isHovered)
				{
					UE_LOG(LogTemp, Warning, TEXT("hover"));
					winState = WindowState::CurveMove;
					selectCurve = &curveList[i];
					selectCurve->Get().isSelected = true;
					clickPos = mouseEvent.GetScreenSpacePosition();
					selectCurvePos = selectCurve->Get().GetRenderTransform()->GetTranslation();
					return FReply::Handled();;
				}
			}
			UE_LOG(LogTemp, Warning, TEXT(" no hover"));
		}


	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("canvas right click"));
		winState = WindowState::CanvasMove;
		clickPos = mouseEvent.GetScreenSpacePosition();
		FSlateApplication::Get().GetPlatformCursor().Get()->SetType(EMouseCursor::GrabHand);
		canvasPos = ctlCanvas->GetRenderTransform()->GetTranslation();
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"),*mouseEvent.GetScreenSpacePosition().ToString());
	return FReply::Handled();
}

FReply SCurvePanel::OnMouseButtonUp(const FGeometry & MyGeometry, const FPointerEvent & mouseEvent)
{
	switch (winState)
	{
	case Normal:
		break;

	case CurveMove:
		UE_LOG(LogTemp, Warning, TEXT("%s"),*selectCurve->Get().canvasSlot->PositionAttr.Get().ToString());
		selectCurve->Get().isSelected = false;
		winState = WindowState::Normal;
		break;

	case CanvasMove:
		winState = WindowState::Normal;
		FSlateApplication::Get().GetPlatformCursor().Get()->SetType(EMouseCursor::Default);
		break;

	default:
		break;
	}

	return FReply::Handled();
}

FReply SCurvePanel::OnMouseMove(const FGeometry & MyGeometry, const FPointerEvent & mouseEvent)
{
	switch (winState)
	{
	case Normal:
		FSlateApplication::Get().GetPlatformCursor().Get()->SetType(EMouseCursor::Default);
		break;

	case CurveMove:
		selectCurve->Get().SetRenderTransform(FSlateRenderTransform(selectCurvePos+(mouseEvent.GetScreenSpacePosition() - clickPos)/canvasScale));
		break;

	case CanvasMove:
		canvasOffset = canvasPos + mouseEvent.GetScreenSpacePosition() - clickPos;
		setCanvasTrans();
		FSlateApplication::Get().GetPlatformCursor().Get()->SetType(EMouseCursor::GrabHandClosed);
		break;

	default:
		break;
	}

	return FReply::Handled();
}

FReply SCurvePanel::OnKeyDown(const FGeometry & MyGeometry, const FKeyEvent & InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::F)
	{
		initParam();
		setCanvasTrans();
	}
	return FReply::Handled();
}

FReply SCurvePanel::testBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("btn clicked1"));
	return FReply::Handled();
}

void SCurvePanel::myDebug(FString _inStr)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"),*_inStr);
}


