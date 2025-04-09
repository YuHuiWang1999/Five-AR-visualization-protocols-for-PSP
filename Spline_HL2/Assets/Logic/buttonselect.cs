using Microsoft.MixedReality.Toolkit.UI;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

public class buttonselect : MonoBehaviour
{
    public int [] numbers;
    public Interactable[] checkbox_1;
    public Interactable[] checkbox_2;
    public Interactable[] checkbox_3;
    public Interactable[] checkbox_4;
    public Interactable[] checkbox_5;
    private Interactable interactable;
    // Start is called before the first frame update
    void Start()
    {
        numbers = new int[5];
        for (int i = 0;i< numbers.Length;i++)
        {
            numbers[i] = 0;
        }
    }
        
    // Update is called once per frame
    void Update()
    {

    }

    public int[] getnumbers()
    {
        return numbers;//
    }
    public void checkboxreset0()
    {
        for (int i = 0; i < numbers.Length; i++) //·¢ËÍºóÒþ²ØÇÒ¶¼¹éÁã
        {
            numbers[i] = 0;
        }
        for (int i = 0; i < checkbox_1.Length; i++) { checkbox_1[i].IsToggled = false; }
        for (int i = 0; i < checkbox_2.Length; i++) { checkbox_2[i].IsToggled = false; }
        for (int i = 0; i < checkbox_3.Length; i++) { checkbox_3[i].IsToggled = false; }
        for (int i = 0; i < checkbox_4.Length; i++) { checkbox_4[i].IsToggled = false; }
        for (int i = 0; i < checkbox_5.Length; i++) { checkbox_5[i].IsToggled = false; }
    }
    public void checkbox1_1() 
    {
        numbers[0] = 1;
        for (int i = 0; i < checkbox_1.Length; i++) { checkbox_1[i].IsToggled = false; }
        checkbox_1[numbers[0] -1].IsToggled = true;
    }
    public void checkbox1_2()
    {
        numbers[0] = 2;
        for (int i = 0; i < checkbox_1.Length; i++) { checkbox_1[i].IsToggled = false; }
        checkbox_1[numbers[0] - 1].IsToggled = true;
    }
    public void checkbox1_3()
    {
        numbers[0] = 3;
        for (int i = 0; i < checkbox_1.Length; i++) { checkbox_1[i].IsToggled = false; }
        checkbox_1[numbers[0] - 1].IsToggled = true;
    }
    public void checkbox1_4()
    {
        numbers[0] = 4;
        for (int i = 0; i < checkbox_1.Length; i++) { checkbox_1[i].IsToggled = false; }
        checkbox_1[numbers[0] - 1].IsToggled = true;
    }
    public void checkbox1_5()
    {
        numbers[0] = 5;
        for (int i = 0; i < checkbox_1.Length; i++) { checkbox_1[i].IsToggled = false; }
        checkbox_1[numbers[0] - 1].IsToggled = true;
    }
    //2
    public void checkbox2_1()
    {
        numbers[1] = 1;
        for (int i = 0; i < checkbox_2.Length; i++) { checkbox_2[i].IsToggled = false; }
        checkbox_2[numbers[1] - 1].IsToggled = true;
    }
    public void checkbox2_2()
    {
        numbers[1] = 2;
        for (int i = 0; i < checkbox_2.Length; i++) { checkbox_2[i].IsToggled = false; }
        checkbox_2[numbers[1] - 1].IsToggled = true;
    }
    public void checkbox2_3()
    {
        numbers[1] = 3;
        for (int i = 0; i < checkbox_2.Length; i++) { checkbox_2[i].IsToggled = false; }
        checkbox_2[numbers[1] - 1].IsToggled = true;
    }
    public void checkbox2_4()
    {
        numbers[1] = 4;
        for (int i = 0; i < checkbox_2.Length; i++) { checkbox_2[i].IsToggled = false; }
        checkbox_2[numbers[1] - 1].IsToggled = true;
    }
    public void checkbox2_5()
    {
        numbers[1] = 5;
        for (int i = 0; i < checkbox_2.Length; i++) { checkbox_2[i].IsToggled = false; }
        checkbox_2[numbers[1] - 1].IsToggled = true;
    }
    public void checkbox3_1()
    {
        numbers[2] = 1;
        for (int i = 0; i < checkbox_3.Length; i++) { checkbox_3[i].IsToggled = false; }
        checkbox_3[numbers[2] - 1].IsToggled = true;
    }
    public void checkbox3_2()
    {
        numbers[2] = 2;
        for (int i = 0; i < checkbox_3.Length; i++) { checkbox_3[i].IsToggled = false; }
        checkbox_3[numbers[2] - 1].IsToggled = true;
    }
    public void checkbox3_3()
    {
        numbers[2] = 3;
        for (int i = 0; i < checkbox_3.Length; i++) { checkbox_3[i].IsToggled = false; }
        checkbox_3[numbers[2] - 1].IsToggled = true;
    }
    public void checkbox3_4()
    {
        numbers[2] = 4;
        for (int i = 0; i < checkbox_3.Length; i++) { checkbox_3[i].IsToggled = false; }
        checkbox_3[numbers[2] - 1].IsToggled = true;
    }
    public void checkbox3_5()
    {
        numbers[2] = 5;
        for (int i = 0; i < checkbox_3.Length; i++) { checkbox_3[i].IsToggled = false; }
        checkbox_3  [numbers[2] - 1].IsToggled = true;
    }
    //4
    public void checkbox4_1()
    {
        numbers[3] = 1;
        for (int i = 0; i < checkbox_4.Length; i++) { checkbox_4[i].IsToggled = false; }
        checkbox_4[numbers[3] - 1].IsToggled = true;
    }
    public void checkbox4_2()
    {
        numbers[3] = 2;
        for (int i = 0; i < checkbox_4.Length; i++) { checkbox_4[i].IsToggled = false; }
        checkbox_4[numbers[3] - 1].IsToggled = true;
    }
    public void checkbox4_3()
    {
        numbers[3] = 3;
        for (int i = 0; i < checkbox_4.Length; i++) { checkbox_4[i].IsToggled = false; }
        checkbox_4[numbers[3] - 1].IsToggled = true;
    }
    public void checkbox4_4()
    {
        numbers[3] = 4;
        for (int i = 0; i < checkbox_4.Length; i++) { checkbox_4[i].IsToggled = false; }
        checkbox_4[numbers[3] - 1].IsToggled = true;
    }
    public void checkbox4_5()
    {
        numbers[3] = 5;
        for (int i = 0; i < checkbox_4.Length; i++) { checkbox_4[i].IsToggled = false; }
        checkbox_4[numbers[3] - 1].IsToggled = true;
    }
    //5
    public void checkbox5_1()
    {
        numbers[4] = 1;
        for (int i = 0; i < checkbox_5.Length; i++) { checkbox_5[i].IsToggled = false; }
        checkbox_5[numbers[4] - 1].IsToggled = true;
    }
    public void checkbox5_2()
    {
        numbers[4] = 2;
        for (int i = 0; i < checkbox_5.Length; i++) { checkbox_5[i].IsToggled = false; }
        checkbox_5[numbers[4] - 1].IsToggled = true;
    }
    public void checkbox5_3()
    {
        numbers[4] = 3;
        for (int i = 0; i < checkbox_5.Length; i++) { checkbox_5[i].IsToggled = false; }
        checkbox_5[numbers[4] - 1].IsToggled = true;
    }
    public void checkbox5_4()
    {
        numbers[4] = 4;
        for (int i = 0; i < checkbox_5.Length; i++) { checkbox_5[i].IsToggled = false; }
        checkbox_5[numbers[4] - 1].IsToggled = true;
    }
    public void checkbox5_5()
    {
        numbers[4] = 5;
        for (int i = 0; i < checkbox_5.Length; i++) { checkbox_5[i].IsToggled = false; }
        checkbox_5[numbers[4] - 1].IsToggled = true;
    }
}
