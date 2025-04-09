using UnityEngine;

public class camera : MonoBehaviour
{
    public GameObject model;
    public Camera renderCamera;
    public Transform projectionPlane;

    private Material projectionMaterial;
    private RenderTexture modelRenderTexture;

    void Start()
    {
        // 创建渲染目标纹理
        modelRenderTexture = new RenderTexture(Screen.width, Screen.height, 24);
        modelRenderTexture.Create();

        // 创建材质
        projectionMaterial = new Material(Shader.Find("Unlit/Texture"));

        // 设置1号相机用于渲染目标模型
        renderCamera.targetTexture = modelRenderTexture;

        // 将材质的主纹理设置为目标模型的RenderTexture
        projectionMaterial.mainTexture = modelRenderTexture;

        // 设置模型的层为“RenderModel”，这是用于渲染模型的层
        model.layer = LayerMask.NameToLayer("RenderModel");

        // 设置1号相机只渲染“RenderModel”层
        renderCamera.cullingMask = 1 << LayerMask.NameToLayer("RenderModel");

        // 将模型投影到平面上
        ProjectModelOutline();
    }

    private void ProjectModelOutline()
    {
        // 在渲染之前将目标模型的渲染纹理设置为当前的目标纹理
        model.GetComponent<Renderer>().material.mainTexture = modelRenderTexture;

        // 渲染目标模型到渲染纹理
        renderCamera.Render();

        // 从渲染纹理中提取模型轮廓
        Texture2D modelOutline = ExtractModelOutline();

        // 将模型轮廓应用于材质
        projectionMaterial.mainTexture = modelOutline;

        // 将材质应用于平面
        projectionPlane.GetComponent<Renderer>().material = projectionMaterial;
    }

    private Texture2D ExtractModelOutline()
    {
        // 将RenderTexture转换为Texture2D
        Texture2D tex = new Texture2D(modelRenderTexture.width, modelRenderTexture.height, TextureFormat.RGBA32, false);
        RenderTexture.active = modelRenderTexture;
        tex.ReadPixels(new Rect(0, 0, modelRenderTexture.width, modelRenderTexture.height), 0, 0);
        tex.Apply();
        RenderTexture.active = null;

        // 将颜色转换为灰度值，并将其存储在alpha通道中
        for (int x = 0; x < tex.width; x++)
        {
            for (int y = 0; y < tex.height; y++)
            {
                Color c = tex.GetPixel(x, y);
                float gray = (c.r + c.g + c.b) / 3.0f;
                c.a = gray;
                tex.SetPixel(x, y, c);
            }
        }
        tex.Apply();

        // 使用轮廓检测算法从灰度图像中提取轮廓
        Texture2D outline = new Texture2D(tex.width, tex.height, TextureFormat.Alpha8, false);
        outline.wrapMode = TextureWrapMode.Clamp;
        Color[] pixels = tex.GetPixels();
        Color[] outlinePixels = new Color[pixels.Length];
        for (int x = 1; x < tex.width - 1; x++)
        {
            for (int y = 1; y < tex.height - 1; y++)
            {
                // 计算像素索引
                int index = y * tex.width + x;

                // 检查像素是否在轮廓上
                bool isOutline = false;
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        int neighborIndex = (y + i) * tex.width + x + j;
                        if (pixels[neighborIndex].a < 1.0f)
                        {
                            isOutline = true;
                            break;
                        }
                    }
                    if (isOutline)
                        break;
                }

                // 设置像素颜色
                if (isOutline)
                    outlinePixels[index] = Color.white;
                else
                    outlinePixels[index] = Color.clear;
            }
        }
        outline.SetPixels(outlinePixels);
        outline.Apply();

        return outline;
    }

}
