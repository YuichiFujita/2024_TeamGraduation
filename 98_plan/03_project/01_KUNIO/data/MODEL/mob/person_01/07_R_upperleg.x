xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 56;
 -5.50464;-9.38517;9.44055;,
 -10.46361;-5.98281;1.66740;,
 -9.85323;-21.41067;1.21722;,
 5.57286;-55.89102;7.01526;,
 6.33912;-44.08191;7.76265;,
 1.15212;-44.68653;8.00133;,
 -2.66958;-56.08542;7.06965;,
 -5.38296;-21.87105;10.12158;,
 -4.11486;-44.68899;7.75068;,
 10.60506;-20.70996;0.83961;,
 9.01629;-9.08388;10.09128;,
 7.42374;-21.39087;10.14393;,
 -5.19045;-56.27076;0.56082;,
 -6.13440;-45.05271;0.67512;,
 -3.43530;-45.07953;-7.14180;,
 -2.13609;-56.70219;-6.60291;,
 3.05967;-44.64171;-8.21160;,
 3.49812;-56.71719;-7.54929;,
 6.13650;-9.14406;-9.72741;,
 5.83551;-21.15450;-8.60214;,
 -0.04302;-21.20997;-10.72278;,
 -0.95991;-7.94475;-11.53131;,
 0.46221;-11.11233;11.53131;,
 -5.50464;-9.38517;9.44055;,
 1.01172;-22.33521;10.75038;,
 -7.38351;-6.69321;-9.00801;,
 -6.04152;-21.58782;-8.22951;,
 8.88960;-55.75263;0.54507;,
 9.04185;-43.83444;0.72585;,
 8.88960;-55.75263;0.54507;,
 3.49812;-56.71719;-7.54929;,
 3.05967;-44.64171;-8.21160;,
 9.04185;-43.83444;0.72585;,
 10.60506;-20.70996;0.83961;,
 9.01629;-9.08388;10.09128;,
 -1.44306;1.24584;6.31323;,
 -3.90228;3.08793;1.60896;,
 -2.73111;2.91051;-4.64112;,
 0.99945;0.40713;7.67730;,
 -1.44306;1.24584;6.31323;,
 -5.50464;-9.38517;9.44055;,
 -2.73111;2.91051;-4.64112;,
 5.65740;1.65801;6.53844;,
 0.99945;0.40713;7.67730;,
 0.46221;-11.11233;11.53131;,
 9.01629;-9.08388;10.09128;,
 0.06360;2.32062;-6.25131;,
 0.06360;2.32062;-6.25131;,
 -0.95991;-7.94475;-11.53131;,
 4.72611;1.65642;-5.61858;,
 6.13650;-9.14406;-9.72741;,
 0.06360;2.32062;-6.25131;,
 -0.95991;-7.94475;-11.53131;,
 3.49665;-56.71674;-7.55508;,
 8.89485;-55.74747;0.54897;,
 8.89485;-55.74747;0.54897;;
 
 59;
 3;0,1,2;,
 3;3,4,5;,
 3;5,6,3;,
 3;0,2,7;,
 3;6,5,8;,
 3;9,10,11;,
 3;12,13,14;,
 3;12,14,15;,
 3;15,14,16;,
 3;15,16,17;,
 3;18,19,20;,
 3;18,20,21;,
 3;22,23,7;,
 3;22,7,24;,
 3;6,8,13;,
 3;6,13,12;,
 3;25,26,2;,
 3;25,2,1;,
 3;21,20,26;,
 3;21,26,25;,
 3;27,28,4;,
 3;27,4,3;,
 3;29,30,31;,
 3;29,31,32;,
 3;20,19,31;,
 3;2,13,8;,
 3;2,8,7;,
 3;24,5,4;,
 3;24,4,11;,
 3;26,14,13;,
 3;26,13,2;,
 3;24,7,8;,
 3;24,8,5;,
 3;11,4,28;,
 3;11,28,9;,
 3;19,33,32;,
 3;19,32,31;,
 3;26,20,16;,
 3;26,16,14;,
 3;11,34,22;,
 3;11,22,24;,
 3;33,19,18;,
 3;18,10,33;,
 4;35,36,1,23;,
 4;36,37,25,1;,
 3;36,35,37;,
 4;38,39,40,22;,
 3;41,39,38;,
 4;42,43,44,45;,
 3;43,42,46;,
 4;37,47,48,25;,
 3;43,47,37;,
 4;49,42,45,50;,
 4;51,49,50,52;,
 3;42,49,51;,
 3;12,15,17;,
 4;6,12,30,53;,
 4;54,29,3,6;,
 4;55,54,6,17;;
 
 MeshMaterialList {
  9;
  59;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.674400;0.574400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.125600;0.125600;0.125600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.019200;0.310400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.248000;0.119200;0.065600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.420000;0.420000;0.420000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.282400;0.282400;0.282400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.592800;0.380000;0.122400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  57;
  -0.776120;0.190561;-0.601102;,
  0.992355;0.121338;0.022530;,
  0.715183;0.132589;-0.686246;,
  -0.947247;0.268380;0.175199;,
  -0.019428;-0.074975;0.996996;,
  0.031092;-0.075446;0.996665;,
  -0.165507;-0.060000;-0.984382;,
  0.995870;-0.030288;-0.085593;,
  -0.987536;-0.098494;-0.122765;,
  -0.464077;-0.091177;-0.881090;,
  -0.654692;0.180066;0.734135;,
  0.005487;-0.092157;0.995729;,
  -0.056538;0.099540;0.993426;,
  -0.124935;0.149983;-0.980763;,
  0.002918;-0.100321;0.994951;,
  0.074325;-0.099039;0.992304;,
  0.940087;-0.089308;0.329030;,
  0.702175;-0.081366;-0.707340;,
  0.993468;-0.061512;-0.096119;,
  0.996120;-0.014126;-0.086868;,
  -0.990752;-0.085748;0.105159;,
  -0.992078;-0.124421;-0.017369;,
  -0.641460;-0.115612;-0.758395;,
  -0.932962;-0.105157;-0.344272;,
  -0.066580;-0.098851;0.992872;,
  -0.517030;-0.047842;0.854629;,
  0.058759;-0.092205;-0.994005;,
  0.035583;-0.090153;-0.995292;,
  -0.599748;0.566777;-0.564859;,
  0.109251;0.330702;0.937390;,
  0.967511;0.228649;-0.107897;,
  -0.808945;0.553495;0.198119;,
  -0.495453;0.421403;0.759569;,
  -0.091692;0.348541;0.932798;,
  0.063920;0.994299;0.085345;,
  0.916332;-0.088847;0.390438;,
  -0.453415;-0.346986;-0.820985;,
  0.069961;-0.994911;0.072512;,
  -0.158044;-0.063257;-0.985404;,
  -0.940846;-0.109653;0.320601;,
  -0.933432;-0.129880;0.334418;,
  0.919033;-0.061895;0.389291;,
  0.887322;-0.086247;0.453013;,
  0.994359;-0.050628;-0.093207;,
  0.831701;-0.001183;-0.555223;,
  0.127773;-0.063647;0.989759;,
  0.141947;0.159007;0.977020;,
  0.507751;0.858592;0.070772;,
  0.389790;0.916156;0.093390;,
  0.349403;0.931586;0.100321;,
  0.106040;0.986018;0.128544;,
  -0.040959;0.996737;0.069553;,
  -0.030350;0.466244;-0.884135;,
  0.234935;0.405022;-0.883608;,
  0.142492;0.989734;-0.011045;,
  0.008012;-0.997949;0.063516;,
  0.039006;-0.996919;0.068048;;
  59;
  3;10,3,20;,
  3;5,15,11;,
  3;11,4,5;,
  3;10,20,25;,
  3;4,11,24;,
  3;19,1,16;,
  3;8,21,23;,
  3;8,23,9;,
  3;9,38,26;,
  3;9,26,6;,
  3;2,17,27;,
  3;2,27,13;,
  3;12,10,25;,
  3;12,25,14;,
  3;39,40,21;,
  3;39,21,8;,
  3;0,22,20;,
  3;0,20,3;,
  3;13,27,22;,
  3;13,22,0;,
  3;7,18,41;,
  3;7,41,42;,
  3;7,43,44;,
  3;7,44,18;,
  3;27,17,26;,
  3;20,21,40;,
  3;20,40,25;,
  3;14,11,15;,
  3;14,15,45;,
  3;22,23,21;,
  3;22,21,20;,
  3;14,25,24;,
  3;14,24,11;,
  3;16,41,18;,
  3;16,18,19;,
  3;17,19,18;,
  3;17,18,26;,
  3;22,27,26;,
  3;22,26,38;,
  3;45,46,12;,
  3;45,12,14;,
  3;19,17,2;,
  3;2,1,19;,
  4;32,31,3,10;,
  4;31,28,0,3;,
  3;47,48,49;,
  4;33,32,10,12;,
  3;49,48,50;,
  4;29,33,12,46;,
  3;50,51,34;,
  4;28,52,13,0;,
  3;50,34,49;,
  4;30,30,1,2;,
  4;52,53,2,13;,
  3;51,54,34;,
  3;55,55,56;,
  4;35,35,43,35;,
  4;36,36,36,36;,
  4;37,37,37,56;;
 }
 MeshTextureCoords {
  56;
  0.401410;0.637760;,
  0.376990;0.627150;,
  0.374750;0.677840;,
  0.428300;0.802410;,
  0.434780;0.760580;,
  0.416820;0.760420;,
  0.397390;0.799890;,
  0.406730;0.681040;,
  0.398760;0.757840;,
  0.480720;0.690390;,
  0.467390;0.651840;,
  0.448540;0.685200;,
  0.371580;0.798190;,
  0.373110;0.758270;,
  0.344540;0.758670;,
  0.344450;0.798560;,
  0.328050;0.759130;,
  0.329960;0.799140;,
  0.293910;0.643860;,
  0.299650;0.683590;,
  0.320190;0.680580;,
  0.317400;0.636380;,
  0.430300;0.648090;,
  0.413340;0.642260;,
  0.427260;0.685260;,
  0.339830;0.629850;,
  0.341690;0.679480;,
  0.453440;0.804280;,
  0.460090;0.765180;,
  0.290870;0.802330;,
  0.317170;0.800760;,
  0.312310;0.760740;,
  0.283280;0.763950;,
  0.264960;0.689460;,
  0.452550;0.647600;,
  0.413340;0.642260;,
  0.376990;0.627150;,
  0.339830;0.629850;,
  0.430300;0.648090;,
  0.200710;0.318880;,
  0.200710;0.318880;,
  0.000000;0.000000;,
  0.233700;0.325920;,
  0.215150;0.324050;,
  0.215150;0.324050;,
  0.233700;0.325920;,
  0.317400;0.636380;,
  0.373850;0.642240;,
  0.373850;0.642240;,
  0.380650;0.647850;,
  0.380650;0.647850;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}