Shader "Unlit/CutoutShader"
{
    Properties{
        _MainTex("Texture", 2D) = "white" {}
    }

        SubShader{
            Tags { "Queue" = "Transparent" } // ������Ⱦ����Ϊ͸�����У�ȷ����ȷ����Ⱦ˳��
            Pass {
                Stencil {
                    Ref 1                // ����Stencil�ο�ֵΪ1
                    Comp always         // ÿ����Ⱦ����Stencilֵ����Ϊ�ο�ֵ
                    Pass replace     // ÿ����Ⱦ�������ǵ�ǰ��Stencilֵ
                }
                CGPROGRAM
                #pragma vertex vert
                #pragma fragment frag
                #include "UnityCG.cginc"

                struct appdata {
                    float4 vertex : POSITION;
                };

                struct v2f {
                    float4 vertex : SV_POSITION;
                };

                sampler2D _MainTex;

                v2f vert(appdata v) {
                    v2f o;
                    o.vertex = UnityObjectToClipPos(v.vertex);
                    return o;
                }

                fixed4 frag(v2f i) : SV_Target {
                    return tex2D(_MainTex, i.vertex.xy);
                }
                ENDCG
            }
    }
}