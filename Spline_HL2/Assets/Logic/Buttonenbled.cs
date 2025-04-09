using Microsoft.MixedReality.Toolkit.UI;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Buttonenbled : MonoBehaviour
{
    private int AcquirclickCount = 0;
    private bool AcquirisClickable;
    public Interactable Acquirbutton;
    public GameObject Acquirbackground;
    private Renderer AcquirbuttonRenderer;
    private Color AcquiroriginalButtonColor;
    public Image[] Acquirimages;             // 采取了几个标定点示意图
    public GameObject RegButton;
    public GameObject ReRegButton;

    private int CliaclickCount = 0;
    private bool CliaisClickable;
    public Interactable Cliabutton;
    public GameObject Cliabackground;
    private Renderer CliabuttonRenderer;
    private Color CliaoriginalButtonColor;
    public Color disabledColor = Color.gray;
    public Image[] cliaimages;             // 采取了几个标定点示意图
    public GameObject CalButton;
    public GameObject ReCalButton;



    // Start is called before the first frame update
    void Start()
    {
        AcquirisClickable = Acquirbutton.IsEnabled;        
        AcquirbuttonRenderer = Acquirbackground.GetComponent<Renderer>();
        AcquiroriginalButtonColor = AcquirbuttonRenderer.material.color;

        CliaisClickable = Cliabutton.IsEnabled;
        CliabuttonRenderer = Cliabackground.GetComponent<Renderer>();
        CliaoriginalButtonColor = CliabuttonRenderer.material.color;

    }

    // Update is called once per frame
    void Update()
    {
        
    }
    public void AcquireOnClick()
    {
        if (AcquirisClickable)
        {
            AcquirclickCount++;//1-8


            if (AcquirclickCount <= 8)
            {
                for (int i = 0; i < Acquirimages.Length; i++)
                {
                    // 根据状态值设置颜色
                    if (i < AcquirclickCount)
                    {
                        // 数值小于state的图片设置为绿色
                        Acquirimages[i].color = Color.green;
                    }
                    else
                    {
                        // 数值大于state的图片设置为灰色
                        Acquirimages[i].color = Color.grey;
                    }
                }
            }

            if (AcquirclickCount >= 8)
            {
                Debug.Log("禁用");
                AcquirisClickable = false;
                Acquirbutton.IsEnabled = false;
                // 修改按钮的颜色为禁用状态的颜色
                AcquirbuttonRenderer.material.color = disabledColor;
            }
        }
    }
    public int getAcqCount()
    {
        return AcquirclickCount;
    }
    public void CliaOnClick()
    {
        if (CliaisClickable)
        {
            CliaclickCount++;

            if (CliaclickCount <= 4)
            {
                for (int i = 0; i < cliaimages.Length; i++)
                {
                    // 根据状态值设置颜色
                    if (i < CliaclickCount)
                    {
                        // 数值小于state的图片设置为绿色
                        cliaimages[i].color = Color.green;
                    }
                    else
                    {
                        // 数值大于state的图片设置为灰色
                        cliaimages[i].color = Color.grey;
                    }
                }
            }

            if (CliaclickCount >= 4)
            {
                CliaisClickable = false;
                Cliabutton.IsEnabled = false;
                // 修改按钮的颜色为禁用状态的颜色
                CliabuttonRenderer.material.color = disabledColor;
            }
        }
    }

    public void AcquirbuttonReset()
    {
        AcquirclickCount = 0;
        AcquirisClickable = true;
        Acquirbutton.IsEnabled = true;
        AcquirbuttonRenderer.material.color = AcquiroriginalButtonColor;
        for (int i = 0; i < Acquirimages.Length; i++)
        {
            Acquirimages[i].color = Color.grey;
        }
    }
    public void RegButtonSet()
    {
        if (AcquirclickCount>=8) 
        {
            RegButton.SetActive(false);
            ReRegButton.SetActive(true);
        }
    }

    public void CliabuttonReset()
    {
        CliaclickCount = 0;
        CliaisClickable = true;
        Cliabutton.IsEnabled = true;
        CliabuttonRenderer.material.color = AcquiroriginalButtonColor;
        for (int i = 0; i < cliaimages.Length; i++)
        {
            cliaimages[i].color = Color.grey;
        }
    }
    public void CalButtonSet()
    {
        if (CliaclickCount >= 4)
        {
            CalButton.SetActive(false);
            ReCalButton.SetActive(true);
        }
    }

    public int getCalCount()
    {
        return CliaclickCount;
    }
}

