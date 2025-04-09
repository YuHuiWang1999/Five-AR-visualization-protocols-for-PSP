using UnityEngine;
using UnityEngine.UI;

public class RadialGradient : MonoBehaviour
{
    public Image myImage;
    public Gradient gradient;
    public float time;

    void Start() {





    }
    void Update()
    {



    }
    //[SerializeField] private Gradient gradient;
    //[SerializeField, Range(0, 1)] private float gradientPosition=0.5f;
    //public Image myimage;

    //private void Awake()
    //{
    //    ApplyGradient();
    //}

    //private void OnValidate()
    //{
    //    ApplyGradient();
    //}

    //private void ApplyGradient()
    //{
    //    var image = GetComponent<Image>();

    //    // 创建一个渐变纹理
    //    var texture = new Texture2D(128, 128, TextureFormat.RGBA32, false);
    //    for (var y = 0; y < texture.height; ++y)
    //    {
    //        for (var x = 0; x < texture.width; ++x)
    //        {
    //            var normalizedRadius = Mathf.Sqrt(Mathf.Pow((x - texture.width * 0.5f) / texture.width, 2f) + Mathf.Pow((y - texture.height * 0.5f) / texture.height, 2f)) * 2;
    //            var color = gradient.Evaluate(Mathf.Clamp01(normalizedRadius - gradientPosition));
    //            texture.SetPixel(x, y, color);
    //        }
    //    }
    //    texture.Apply();

    //    // 创建一个新的Sprite并应用渐变纹理
    //    var sprite = Sprite.Create(texture, new Rect(0, 0, texture.width, texture.height), Vector2.one * 0.5f);
    //    image.sprite = sprite;
    //}
}
