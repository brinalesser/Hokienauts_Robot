using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class Example : MonoBehaviour
{
    public SimplestPlot.PlotType PlotExample = SimplestPlot.PlotType.TimeSeries;
    public int DataPoints = 100;
    private SimplestPlot SimplestPlotScript;
    private float Counter = 0;
    private Color[] MyColors = new Color[10];

    private System.Random MyRandom;
    private float[] XValues;
    private float[] Y1Values;
    private float[] Y2Values;
    private float[] Y3Values;
    private float[] Y4Values;
    private float[] Y5Values;
    private float[] Y6Values;
    private float[] Y7Values;
    private float[] Y8Values;
    private float[] Y9Values;
    private float[] Y10Values;
    

    private Vector2 Resolution;
    // Use this for initialization
    void Start()
    {
        SimplestPlotScript = GetComponent<SimplestPlot>();

        MyColors[0] = new Color(118, 0, 237); //415
        MyColors[1] = new Color(0, 40, 255); //445
        MyColors[2] = new Color(0, 213, 255); //480
        MyColors[3] = new Color(31, 255, 0); //515
        MyColors[4] = new Color(179, 255, 0); //555
        MyColors[5] = new Color(255, 223, 0); //590
        MyColors[6] = new Color(255, 79, 0); //630
        MyColors[7] = new Color(255, 0, 0); //680
        MyColors[8] = new Color(100, 100, 100); //Clear
        MyColors[9] = new Color(255, 255, 255); //NIR

        XValues = new float[DataPoints];
        Y1Values = new float[DataPoints];
        Y2Values = new float[DataPoints];
        Y3Values = new float[DataPoints];
        Y4Values = new float[DataPoints];
        Y5Values = new float[DataPoints];
        Y6Values = new float[DataPoints];
        Y7Values = new float[DataPoints];
        Y8Values = new float[DataPoints];
        Y9Values = new float[DataPoints];
        Y10Values = new float[DataPoints];

        MyRandom = new System.Random();

        SimplestPlotScript.SetResolution(new Vector2(300, 300));
        SimplestPlotScript.BackGroundColor = new Color(255, 255, 255);
        SimplestPlotScript.TextColor = Color.yellow;
        for (int Cnt = 0; Cnt < 10; Cnt++)
        {
            SimplestPlotScript.SeriesPlotY.Add(new SimplestPlot.SeriesClass());
            SimplestPlotScript.DistributionPlot.Add(new SimplestPlot.DistributionClass());
            SimplestPlotScript.PhaseSpacePlot.Add(new SimplestPlot.PhaseSpaceClass());

            SimplestPlotScript.SeriesPlotY[Cnt].MyColor = MyColors[Cnt];
            SimplestPlotScript.DistributionPlot[Cnt].MyColor = MyColors[Cnt];
            SimplestPlotScript.PhaseSpacePlot[Cnt].MyColor = MyColors[Cnt];

            SimplestPlotScript.DistributionPlot[Cnt].NumberOfBins = (Cnt + 1) * 5;
        }

        Resolution = SimplestPlotScript.GetResolution();
    }

    // Update is called once per frame
    void Update()
    {
        Counter++;
        PrepareArrays();
        SimplestPlotScript.MyPlotType = PlotExample;
        switch (PlotExample)
        {
            case SimplestPlot.PlotType.TimeSeries:
                SimplestPlotScript.SeriesPlotY[0].YValues = Y1Values;
                SimplestPlotScript.SeriesPlotY[1].YValues = Y2Values;
                SimplestPlotScript.SeriesPlotY[2].YValues = Y3Values;
                SimplestPlotScript.SeriesPlotY[3].YValues = Y4Values;
                SimplestPlotScript.SeriesPlotY[4].YValues = Y5Values;
                SimplestPlotScript.SeriesPlotY[5].YValues = Y6Values;
                SimplestPlotScript.SeriesPlotY[6].YValues = Y7Values;
                SimplestPlotScript.SeriesPlotY[7].YValues = Y8Values;
                SimplestPlotScript.SeriesPlotY[8].YValues = Y9Values;
                SimplestPlotScript.SeriesPlotY[9].YValues = Y10Values;
                SimplestPlotScript.SeriesPlotX = XValues;
                break;
            case SimplestPlot.PlotType.Distribution:
                SimplestPlotScript.SeriesPlotY[0].YValues = Y1Values;
                SimplestPlotScript.SeriesPlotY[1].YValues = Y2Values;
                SimplestPlotScript.SeriesPlotY[2].YValues = Y3Values;
                SimplestPlotScript.SeriesPlotY[3].YValues = Y4Values;
                SimplestPlotScript.SeriesPlotY[4].YValues = Y5Values;
                SimplestPlotScript.SeriesPlotY[5].YValues = Y6Values;
                SimplestPlotScript.SeriesPlotY[6].YValues = Y7Values;
                SimplestPlotScript.SeriesPlotY[7].YValues = Y8Values;
                SimplestPlotScript.SeriesPlotY[8].YValues = Y9Values;
                SimplestPlotScript.SeriesPlotY[9].YValues = Y10Values;
                break;
            case SimplestPlot.PlotType.PhaseSpace:
                SimplestPlotScript.SeriesPlotY[0].YValues = Y1Values;
                SimplestPlotScript.SeriesPlotY[1].YValues = Y2Values;
                SimplestPlotScript.SeriesPlotY[2].YValues = Y3Values;
                SimplestPlotScript.SeriesPlotY[3].YValues = Y4Values;
                SimplestPlotScript.SeriesPlotY[4].YValues = Y5Values;
                SimplestPlotScript.SeriesPlotY[5].YValues = Y6Values;
                SimplestPlotScript.SeriesPlotY[6].YValues = Y7Values;
                SimplestPlotScript.SeriesPlotY[7].YValues = Y8Values;
                SimplestPlotScript.SeriesPlotY[8].YValues = Y9Values;
                SimplestPlotScript.SeriesPlotY[9].YValues = Y10Values;
                break;
            default:
                break;
        }
        SimplestPlotScript.UpdatePlot();
    }
    private void PrepareArrays()
    {
        XValues = RobotMovement.XValues;
        Y1Values = RobotMovement.Y1Values;
        Y2Values = RobotMovement.Y2Values;
        Y3Values = RobotMovement.Y3Values;
        Y4Values = RobotMovement.Y4Values;
        Y5Values = RobotMovement.Y5Values;
        Y6Values = RobotMovement.Y6Values;
        Y7Values = RobotMovement.Y7Values;
        Y8Values = RobotMovement.Y8Values;
        Y9Values = RobotMovement.Y9Values;
        Y10Values = RobotMovement.Y10Values;
    }
}
