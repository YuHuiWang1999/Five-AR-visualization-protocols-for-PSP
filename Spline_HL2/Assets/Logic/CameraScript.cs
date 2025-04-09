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

        // 请求摄像头权限
        yield return Application.RequestUserAuthorization(UserAuthorization.WebCam);
        // 如果获取到摄像头权限
        if (Application.HasUserAuthorization(UserAuthorization.WebCam))
        {
            // 获取所有的摄像头设备
            WebCamDevice[] devices = WebCamTexture.devices;
            if (devices != null)
            {
                // 索引为0的摄像头一般为后置摄像头，参数分别为设备名称、图像宽度、高度、刷新率
                webCamTex = new WebCamTexture(devices[0].name, 800, 1280, 30);
                // 实时获取摄像头的画面
                webCamTex.Play();

                cameraImage.texture = webCamTex;
            }
        }
    }
}