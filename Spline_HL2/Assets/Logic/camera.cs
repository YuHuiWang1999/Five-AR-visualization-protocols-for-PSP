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
        // ������ȾĿ������
        modelRenderTexture = new RenderTexture(Screen.width, Screen.height, 24);
        modelRenderTexture.Create();

        // ��������
        projectionMaterial = new Material(Shader.Find("Unlit/Texture"));

        // ����1�����������ȾĿ��ģ��
        renderCamera.targetTexture = modelRenderTexture;

        // �����ʵ�����������ΪĿ��ģ�͵�RenderTexture
        projectionMaterial.mainTexture = modelRenderTexture;

        // ����ģ�͵Ĳ�Ϊ��RenderModel��������������Ⱦģ�͵Ĳ�
        model.layer = LayerMask.NameToLayer("RenderModel");

        // ����1�����ֻ��Ⱦ��RenderModel����
        renderCamera.cullingMask = 1 << LayerMask.NameToLayer("RenderModel");

        // ��ģ��ͶӰ��ƽ����
        ProjectModelOutline();
    }

    private void ProjectModelOutline()
    {
        // ����Ⱦ֮ǰ��Ŀ��ģ�͵���Ⱦ��������Ϊ��ǰ��Ŀ������
        model.GetComponent<Renderer>().material.mainTexture = modelRenderTexture;

        // ��ȾĿ��ģ�͵���Ⱦ����
        renderCamera.Render();

        // ����Ⱦ��������ȡģ������
        Texture2D modelOutline = ExtractModelOutline();

        // ��ģ������Ӧ���ڲ���
        projectionMaterial.mainTexture = modelOutline;

        // ������Ӧ����ƽ��
        projectionPlane.GetComponent<Renderer>().material = projectionMaterial;
    }

    private Texture2D ExtractModelOutline()
    {
        // ��RenderTextureת��ΪTexture2D
        Texture2D tex = new Texture2D(modelRenderTexture.width, modelRenderTexture.height, TextureFormat.RGBA32, false);
        RenderTexture.active = modelRenderTexture;
        tex.ReadPixels(new Rect(0, 0, modelRenderTexture.width, modelRenderTexture.height), 0, 0);
        tex.Apply();
        RenderTexture.active = null;

        // ����ɫת��Ϊ�Ҷ�ֵ��������洢��alphaͨ����
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

        // ʹ����������㷨�ӻҶ�ͼ������ȡ����
        Texture2D outline = new Texture2D(tex.width, tex.height, TextureFormat.Alpha8, false);
        outline.wrapMode = TextureWrapMode.Clamp;
        Color[] pixels = tex.GetPixels();
        Color[] outlinePixels = new Color[pixels.Length];
        for (int x = 1; x < tex.width - 1; x++)
        {
            for (int y = 1; y < tex.height - 1; y++)
            {
                // ������������
                int index = y * tex.width + x;

                // ��������Ƿ���������
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

                // ����������ɫ
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
