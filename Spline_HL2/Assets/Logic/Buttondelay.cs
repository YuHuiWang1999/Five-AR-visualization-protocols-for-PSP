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
        // ���õڶ�����ť
        button.IsEnabled = false;
        originalButtonColor = buttonRenderer.material.color;
        // �޸İ�ť����ɫΪ����״̬����ɫ
        buttonRenderer.material.color = disabledColor;

        // �ȴ�һ��ʱ��
        yield return new WaitForSeconds(delayTime);

        // ���õڶ�����ť
        button.IsEnabled = true;
        // ��ԭ��ť2����ɫΪԭʼ��ɫ
        buttonRenderer.material.color = originalButtonColor;
    }
}