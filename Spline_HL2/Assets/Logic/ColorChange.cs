using UnityEngine.UI;
using UnityEngine;

namespace Microsoft.MixedReality.Toolkit.UI
{
    /// <summary>
    /// Change the color of the material on a UnityUI Graphic (ex. Image) based on a numeric value. Useful for visualizing button presses.
    /// </summary>
    [AddComponentMenu("Scripts/MRTK/Examples/ColorChangerUnityUI")]
    public class ColorChange : MonoBehaviour
    {
        public GameObject communication;
        private float[] colorValues ;
        public Image[] UIImages;
        public Sprite[] UiSprite;
        private void Start()
        {
            colorValues = new float[3];
        }
        void Update()
        {
            colorValues = communication.GetComponent<Communication>().getClolrChange();//communication中传来是否可见的信号
            SetColor();
        }

        /// <summary>
        /// Sets the color based on the colorValue.
        /// </summary>
        public void SetColor()
        {
            //
            if (colorValues[0]== 0)
            {
                UIImages[0].sprite = UiSprite[0];
            }
            else if(colorValues[0] == 1)
            {
                UIImages[0].sprite = UiSprite[1];
            }
            //
            if (colorValues[1]== 0)
            {
                UIImages[1].sprite = UiSprite[2];
            }
            else if (colorValues[1] == 1)
            {
                UIImages[1].sprite = UiSprite[3];
            }
            //
            if (colorValues[2]== 0)
            {
                UIImages[2].sprite = UiSprite[4];
            }
            else if (colorValues[2] == 1)
            {
                UIImages[2].sprite = UiSprite[5];
            }
            if (colorValues[4] == 0)
            {
                UIImages[3].sprite = UiSprite[6];
            }
            else if (colorValues[4] == 1)
            {
                UIImages[3].sprite = UiSprite[7];
            }
            //
        }
    }
}
