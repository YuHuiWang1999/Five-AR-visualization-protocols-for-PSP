using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CameraScript : MonoBehaviour
{
    RawImage cameraImage;
    private WebCamTexture webCamTex;

    IEnumerator Start()
    {
        cameraImage = GetComponent<RawImage>();

        // ��������ͷȨ��
        yield return Application.RequestUserAuthorization(UserAuthorization.WebCam);
        // �����ȡ������ͷȨ��
        if (Application.HasUserAuthorization(UserAuthorization.WebCam))
        {
            // ��ȡ���е�����ͷ�豸
            WebCamDevice[] devices = WebCamTexture.devices;
            if (devices != null)
            {
                // ����Ϊ0������ͷһ��Ϊ��������ͷ�������ֱ�Ϊ�豸���ơ�ͼ���ȡ��߶ȡ�ˢ����
                webCamTex = new WebCamTexture(devices[0].name, 800, 1280, 30);
                // ʵʱ��ȡ����ͷ�Ļ���
                webCamTex.Play();

                cameraImage.texture = webCamTex;
            }
        }
    }
}