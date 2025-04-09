using Microsoft.MixedReality.Toolkit.Input;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem.HID;
using UnityEngine.UIElements;
using static UnityEngine.GraphicsBuffer;
using Image = UnityEngine.UI.Image;

public class image : MonoBehaviour
{
    // Start is called before the first frame update

    public Material[] V2DYImage;                    // 投影深度图像
    public GameObject V2DY;
    private int index=0;
    public GameObject guide;
    void Start()
    {
        guide.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void change()
    {
        if (index < 4)
        {
            guide.SetActive(false);
            MeshRenderer render = V2DY.GetComponent<MeshRenderer>();
            render.material = V2DYImage[index];
            index++;
        }
        else 
        { index = 0; }

     }
}
