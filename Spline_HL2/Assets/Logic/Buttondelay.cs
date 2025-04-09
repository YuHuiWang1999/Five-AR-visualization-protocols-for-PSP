using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Microsoft.MixedReality.Toolkit.UI;

public class Buttondelay : MonoBehaviour
{

    public Interactable button;
    public float delayTime;
    public Color disabledColor = Color.gray;
    private Color originalButtonColor;
    public GameObject buttonback;
    private Renderer buttonRenderer;

    private void Start()
    {
        buttonRenderer = buttonback.GetComponent<Renderer>();
    }

    public void OnButton1Clicked()
    {
        StartCoroutine(EnableButton2AfterDelay());
    }

    private IEnumerator EnableButton2AfterDelay()
    {
        // 禁用第二个按钮
        button.IsEnabled = false;
        originalButtonColor = buttonRenderer.material.color;
        // 修改按钮的颜色为禁用状态的颜色
        buttonRenderer.material.color = disabledColor;

        // 等待一段时间
        yield return new WaitForSeconds(delayTime);

        // 启用第二个按钮
        button.IsEnabled = true;
        // 还原按钮2的颜色为原始颜色
        buttonRenderer.material.color = originalButtonColor;
    }
}