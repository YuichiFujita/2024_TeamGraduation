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
 268;
 -26.15058;195.86635;0.00000;,
 -21.95015;195.86635;1.13311;,
 -22.38461;195.86635;-2.18903;,
 -26.15058;195.86635;0.00000;,
 -23.97628;195.86635;3.79148;,
 -26.15058;195.86635;0.00000;,
 -27.27606;195.86635;4.22888;,
 -26.15058;195.86635;0.00000;,
 -29.91655;195.86635;2.18903;,
 -26.15058;195.86635;0.00000;,
 -30.35100;195.86635;-1.13311;,
 -26.15058;195.86635;0.00000;,
 -28.32488;195.86635;-3.79148;,
 -26.15058;195.86635;0.00000;,
 -25.02509;195.86635;-4.22888;,
 -26.15058;195.86635;0.00000;,
 -22.38461;195.86635;-2.18903;,
 -26.15058;-4.13367;0.00000;,
 -22.36577;-4.13367;-2.19998;,
 -21.92918;-4.13367;1.13877;,
 -26.15058;-4.13367;0.00000;,
 -23.96540;-4.13367;3.81044;,
 -26.15058;-4.13367;0.00000;,
 -27.28168;-4.13367;4.25000;,
 -26.15058;-4.13367;0.00000;,
 -29.93538;-4.13367;2.19998;,
 -26.15058;-4.13367;0.00000;,
 -30.37198;-4.13367;-1.13877;,
 -26.15058;-4.13367;0.00000;,
 -28.33575;-4.13367;-3.81044;,
 -26.15058;-4.13367;0.00000;,
 -25.01947;-4.13367;-4.25000;,
 -26.15058;-4.13367;0.00000;,
 -22.36577;-4.13367;-2.19998;,
 -23.21581;0.56203;-1.70589;,
 -22.87726;0.56203;0.88302;,
 -23.21581;0.56203;-1.70589;,
 -25.27350;0.56203;-3.29549;,
 -24.45616;0.56203;2.95465;,
 -27.84499;0.56203;-2.95465;,
 -27.02765;0.56203;3.29549;,
 -29.42389;0.56203;-0.88302;,
 -29.08535;0.56203;1.70589;,
 -23.21581;192.60700;-1.70589;,
 -22.87726;192.60700;0.88302;,
 -23.21581;192.60700;-1.70589;,
 -25.27350;192.60700;-3.29549;,
 -24.45616;192.60700;2.95465;,
 -27.84499;192.60700;-2.95465;,
 -27.02765;192.60700;3.29549;,
 -29.42389;192.60700;-0.88302;,
 -29.08535;192.60700;1.70589;,
 51.09733;182.13026;0.00000;,
 51.09733;180.19672;1.95656;,
 51.09733;179.39582;0.00000;,
 51.09733;182.13026;0.00000;,
 51.09733;182.13031;2.76698;,
 51.09733;182.13026;0.00000;,
 51.09733;184.06386;1.95656;,
 51.09733;182.13026;0.00000;,
 51.09733;184.86479;0.00000;,
 51.09733;182.13026;0.00000;,
 51.09733;184.06386;-1.95656;,
 51.09733;182.13026;0.00000;,
 51.09733;182.13031;-2.76698;,
 51.09733;182.13026;0.00000;,
 51.09733;180.19672;-1.95656;,
 51.09733;182.13026;0.00000;,
 51.09733;179.39582;0.00000;,
 -33.90268;182.13031;0.00000;,
 -33.90268;179.38212;0.00000;,
 -33.90268;180.18705;1.96632;,
 -33.90268;182.13031;0.00000;,
 -33.90268;182.13031;2.78077;,
 -33.90268;182.13031;0.00000;,
 -33.90268;184.07355;1.96632;,
 -33.90268;182.13031;0.00000;,
 -33.90268;184.87846;0.00000;,
 -33.90268;182.13031;0.00000;,
 -33.90268;184.07355;-1.96632;,
 -33.90268;182.13031;0.00000;,
 -33.90268;182.13031;-2.78077;,
 -33.90268;182.13031;0.00000;,
 -33.90268;180.18705;-1.96632;,
 -33.90268;182.13031;0.00000;,
 -33.90268;179.38212;0.00000;,
 -29.23000;179.99933;0.00000;,
 -29.23000;180.62345;1.52469;,
 -29.23000;179.99933;0.00000;,
 -29.23000;180.62350;-1.52469;,
 -29.23000;182.13031;2.15627;,
 -29.23000;182.13031;-2.15623;,
 -29.23000;183.63710;1.52469;,
 -29.23000;183.63710;-1.52469;,
 -29.23000;184.26125;0.00000;,
 47.85400;179.99933;0.00000;,
 47.85400;180.62345;1.52469;,
 47.85400;179.99933;0.00000;,
 47.85392;180.62350;-1.52469;,
 47.85400;182.13031;2.15627;,
 47.85392;182.13031;-2.15623;,
 47.85400;183.63710;1.52469;,
 47.85392;183.63710;-1.52469;,
 47.85400;184.26125;0.00000;,
 -17.77543;176.13676;0.00000;,
 41.70129;176.13676;0.00000;,
 41.70129;50.24573;0.00000;,
 -17.77543;50.24573;0.00000;,
 -12.52453;176.09843;-0.47105;,
 -12.52453;171.07179;-0.47105;,
 -24.19118;171.07179;-0.47105;,
 -24.19118;176.09843;-0.47105;,
 -12.52453;171.07179;-0.47105;,
 -12.52453;171.07179;0.47105;,
 -24.19118;171.07179;0.47105;,
 -24.19118;171.07179;-0.47105;,
 -12.52453;171.07179;0.47105;,
 -12.52453;176.09843;0.47105;,
 -24.19118;176.09843;0.47105;,
 -24.19118;171.07179;0.47105;,
 -12.52453;176.09843;0.47105;,
 -12.52453;176.09843;-0.47105;,
 -24.19118;176.09843;-0.47105;,
 -24.19118;176.09843;0.47105;,
 -12.52453;171.07179;-0.47105;,
 -12.52453;176.09843;-0.47105;,
 -24.19118;176.09843;-0.47105;,
 -24.19118;171.07179;-0.47105;,
 -12.48783;171.12724;-0.47105;,
 -17.53991;171.12724;-0.47105;,
 -17.53991;182.38261;-0.47105;,
 -12.48783;182.38261;-0.47105;,
 -17.53991;171.12724;-0.47105;,
 -17.53991;171.12724;0.47105;,
 -17.53991;182.38261;0.47105;,
 -17.53991;182.38261;-0.47105;,
 -17.53991;171.12724;0.47105;,
 -12.48783;171.12724;0.47105;,
 -12.48783;182.38261;0.47105;,
 -17.53991;182.38261;0.47105;,
 -12.48783;171.12724;0.47105;,
 -12.48783;171.12724;-0.47105;,
 -12.48783;182.38261;-0.47105;,
 -12.48783;182.38261;0.47105;,
 -17.53991;171.12724;-0.47105;,
 -12.48783;171.12724;-0.47105;,
 -12.48783;182.38261;-0.47105;,
 -17.53991;182.38261;-0.47105;,
 -12.52453;55.49092;-0.47105;,
 -12.52453;50.46430;-0.47105;,
 -24.19118;50.46430;-0.47105;,
 -24.19118;55.49092;-0.47105;,
 -12.52453;50.46430;-0.47105;,
 -12.52453;50.46430;0.47105;,
 -24.19118;50.46430;0.47105;,
 -24.19118;50.46430;-0.47105;,
 -12.52453;50.46430;0.47105;,
 -12.52453;55.49092;0.47105;,
 -24.19118;55.49092;0.47105;,
 -24.19118;50.46430;0.47105;,
 -12.52453;55.49092;0.47105;,
 -12.52453;55.49092;-0.47105;,
 -24.19118;55.49092;-0.47105;,
 -24.19118;55.49092;0.47105;,
 -12.52453;50.46430;-0.47105;,
 -12.52453;55.49092;-0.47105;,
 -24.19118;55.49092;-0.47105;,
 -24.19118;50.46430;-0.47105;,
 -12.52453;113.18577;-0.47105;,
 -12.52453;108.15913;-0.47105;,
 -24.19118;108.15913;-0.47105;,
 -24.19118;113.18577;-0.47105;,
 -12.52453;108.15913;-0.47105;,
 -12.52453;108.15913;0.47105;,
 -24.19118;108.15913;0.47105;,
 -24.19118;108.15913;-0.47105;,
 -12.52453;108.15913;0.47105;,
 -12.52453;113.18577;0.47105;,
 -24.19118;113.18577;0.47105;,
 -24.19118;108.15913;0.47105;,
 -12.52453;113.18577;0.47105;,
 -12.52453;113.18577;-0.47105;,
 -24.19118;113.18577;-0.47105;,
 -24.19118;113.18577;0.47105;,
 -12.52453;108.15913;-0.47105;,
 -12.52453;113.18577;-0.47105;,
 -24.19118;113.18577;-0.47105;,
 -24.19118;108.15913;-0.47105;,
 -12.52453;82.49308;-0.47105;,
 -12.52453;77.46645;-0.47105;,
 -24.19118;77.46645;-0.47105;,
 -24.19118;82.49308;-0.47105;,
 -12.52453;77.46645;-0.47105;,
 -12.52453;77.46645;0.47105;,
 -24.19118;77.46645;0.47105;,
 -24.19118;77.46645;-0.47105;,
 -12.52453;77.46645;0.47105;,
 -12.52453;82.49308;0.47105;,
 -24.19118;82.49308;0.47105;,
 -24.19118;77.46645;0.47105;,
 -12.52453;82.49308;0.47105;,
 -12.52453;82.49308;-0.47105;,
 -24.19118;82.49308;-0.47105;,
 -24.19118;82.49308;0.47105;,
 -12.52453;77.46645;-0.47105;,
 -12.52453;82.49308;-0.47105;,
 -24.19118;82.49308;-0.47105;,
 -24.19118;77.46645;-0.47105;,
 -12.52453;142.01308;-0.47105;,
 -12.52453;136.98639;-0.47105;,
 -24.19118;136.98639;-0.47105;,
 -24.19118;142.01308;-0.47105;,
 -12.52453;136.98639;-0.47105;,
 -12.52453;136.98639;0.47105;,
 -24.19118;136.98639;0.47105;,
 -24.19118;136.98639;-0.47105;,
 -12.52453;136.98639;0.47105;,
 -12.52453;142.01308;0.47105;,
 -24.19118;142.01308;0.47105;,
 -24.19118;136.98639;0.47105;,
 -12.52453;142.01308;0.47105;,
 -12.52453;142.01308;-0.47105;,
 -24.19118;142.01308;-0.47105;,
 -24.19118;142.01308;0.47105;,
 -12.52453;136.98639;-0.47105;,
 -12.52453;142.01308;-0.47105;,
 -24.19118;142.01308;-0.47105;,
 -24.19118;136.98639;-0.47105;,
 41.60162;171.12724;-0.47105;,
 36.54950;171.12724;-0.47105;,
 36.54954;182.38261;-0.47105;,
 41.60162;182.38261;-0.47105;,
 36.54950;171.12724;-0.47105;,
 36.54950;171.12724;0.47105;,
 36.54954;182.38261;0.47105;,
 36.54954;182.38261;-0.47105;,
 36.54950;171.12724;0.47105;,
 41.60162;171.12724;0.47105;,
 41.60162;182.38261;0.47105;,
 36.54954;182.38261;0.47105;,
 41.60162;171.12724;0.47105;,
 41.60162;171.12724;-0.47105;,
 41.60162;182.38261;-0.47105;,
 41.60162;182.38261;0.47105;,
 36.54950;171.12724;-0.47105;,
 41.60162;171.12724;-0.47105;,
 41.60162;182.38261;-0.47105;,
 36.54954;182.38261;-0.47105;,
 13.02357;171.12724;-0.47105;,
 7.97145;171.12724;-0.47105;,
 7.97149;182.38261;-0.47105;,
 13.02357;182.38261;-0.47105;,
 7.97145;171.12724;-0.47105;,
 7.97145;171.12724;0.47105;,
 7.97149;182.38261;0.47105;,
 7.97149;182.38261;-0.47105;,
 7.97145;171.12724;0.47105;,
 13.02357;171.12724;0.47105;,
 13.02357;182.38261;0.47105;,
 7.97149;182.38261;0.47105;,
 13.02357;171.12724;0.47105;,
 13.02357;171.12724;-0.47105;,
 13.02357;182.38261;-0.47105;,
 13.02357;182.38261;0.47105;,
 7.97145;171.12724;-0.47105;,
 13.02357;171.12724;-0.47105;,
 13.02357;182.38261;-0.47105;,
 7.97149;182.38261;-0.47105;;
 
 130;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 3;17,18,19;,
 3;20,19,21;,
 3;22,21,23;,
 3;24,23,25;,
 3;26,25,27;,
 3;28,27,29;,
 3;30,29,31;,
 3;32,31,33;,
 4;34,35,19,18;,
 4;36,33,31,37;,
 4;35,38,21,19;,
 4;37,31,29,39;,
 4;38,40,23,21;,
 4;39,29,27,41;,
 4;40,42,25,23;,
 4;41,27,25,42;,
 4;43,2,1,44;,
 4;43,44,35,34;,
 4;45,36,37,46;,
 4;45,46,14,16;,
 4;44,1,4,47;,
 4;44,47,38,35;,
 4;46,37,39,48;,
 4;46,48,12,14;,
 4;47,4,6,49;,
 4;47,49,40,38;,
 4;48,39,41,50;,
 4;48,50,10,12;,
 4;49,6,8,51;,
 4;49,51,42,40;,
 4;50,41,42,51;,
 4;50,51,8,10;,
 3;52,53,54;,
 3;55,56,53;,
 3;57,58,56;,
 3;59,60,58;,
 3;61,62,60;,
 3;63,64,62;,
 3;65,66,64;,
 3;67,68,66;,
 3;69,70,71;,
 3;72,71,73;,
 3;74,73,75;,
 3;76,75,77;,
 3;78,77,79;,
 3;80,79,81;,
 3;82,81,83;,
 3;84,83,85;,
 4;86,87,71,70;,
 4;88,85,83,89;,
 4;87,90,73,71;,
 4;89,83,81,91;,
 4;90,92,75,73;,
 4;91,81,79,93;,
 4;92,94,77,75;,
 4;93,79,77,94;,
 4;95,54,53,96;,
 4;95,96,87,86;,
 4;97,88,89,98;,
 4;97,98,66,68;,
 4;96,53,56,99;,
 4;96,99,90,87;,
 4;98,89,91,100;,
 4;98,100,64,66;,
 4;99,56,58,101;,
 4;99,101,92,90;,
 4;100,91,93,102;,
 4;100,102,62,64;,
 4;101,58,60,103;,
 4;101,103,94,92;,
 4;102,93,94,103;,
 4;102,103,60,62;,
 4;104,105,106,107;,
 4;107,106,105,104;,
 4;108,109,110,111;,
 4;112,113,114,115;,
 4;116,117,118,119;,
 4;120,121,122,123;,
 4;120,113,124,125;,
 4;126,127,114,123;,
 4;128,129,130,131;,
 4;132,133,134,135;,
 4;136,137,138,139;,
 4;140,141,142,143;,
 4;140,133,144,145;,
 4;146,147,134,143;,
 4;148,149,150,151;,
 4;152,153,154,155;,
 4;156,157,158,159;,
 4;160,161,162,163;,
 4;160,153,164,165;,
 4;166,167,154,163;,
 4;168,169,170,171;,
 4;172,173,174,175;,
 4;176,177,178,179;,
 4;180,181,182,183;,
 4;180,173,184,185;,
 4;186,187,174,183;,
 4;188,189,190,191;,
 4;192,193,194,195;,
 4;196,197,198,199;,
 4;200,201,202,203;,
 4;200,193,204,205;,
 4;206,207,194,203;,
 4;208,209,210,211;,
 4;212,213,214,215;,
 4;216,217,218,219;,
 4;220,221,222,223;,
 4;220,213,224,225;,
 4;226,227,214,223;,
 4;228,229,230,231;,
 4;232,233,234,235;,
 4;236,237,238,239;,
 4;240,241,242,243;,
 4;240,233,244,245;,
 4;246,247,234,243;,
 4;248,249,250,251;,
 4;252,253,254,255;,
 4;256,257,258,259;,
 4;260,261,262,263;,
 4;260,253,264,265;,
 4;266,267,254,263;;
 
 MeshMaterialList {
  3;
  130;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\wood.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\Pharmacy.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  72;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.862435;0.103174;-0.495540;,
  0.961093;0.103050;0.256282;,
  0.258577;0.103640;-0.960415;,
  0.499110;0.103515;0.860334;,
  -0.499110;0.103515;-0.860334;,
  -0.258577;0.103640;0.960415;,
  -0.961093;0.103050;-0.256282;,
  -0.862436;0.103174;0.495540;,
  0.858115;-0.143346;-0.493042;,
  0.956284;-0.143178;0.254992;,
  0.257275;-0.143979;-0.955552;,
  0.496601;-0.143810;0.855982;,
  -0.496602;-0.143809;-0.855982;,
  -0.257276;-0.143979;0.955552;,
  -0.956284;-0.143178;-0.254992;,
  -0.858115;-0.143347;0.493042;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.065658;-0.997842;-0.000010;,
  0.066040;-0.708512;0.702602;,
  0.066043;-0.708502;-0.702612;,
  0.066429;0.000007;0.997791;,
  0.066431;0.000003;-0.997791;,
  0.066041;0.708515;0.702599;,
  0.066043;0.708508;-0.702605;,
  0.065659;0.997842;0.000000;,
  -0.091970;-0.995762;-0.000009;,
  -0.092504;-0.707029;0.701109;,
  -0.092506;-0.707019;-0.701118;,
  -0.093046;0.000008;0.995662;,
  -0.093046;0.000003;-0.995662;,
  -0.092505;0.707029;0.701108;,
  -0.092506;0.707024;-0.701113;,
  -0.091972;0.995762;0.000001;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000004;0.000000;,
  -1.000000;0.000004;0.000000;,
  0.945780;0.204693;0.252190;,
  0.848667;0.204936;-0.487611;,
  0.254418;0.205843;-0.944934;,
  0.491099;0.205600;0.846493;,
  -0.491099;0.205600;-0.846493;,
  -0.254418;0.205843;0.944934;,
  -0.945780;0.204694;-0.252190;,
  -0.848667;0.204936;0.487611;,
  0.831659;-0.282920;-0.477808;,
  0.926858;-0.282600;0.247128;,
  0.249287;-0.284124;-0.925813;,
  0.481210;-0.283801;0.829394;,
  -0.481210;-0.283800;-0.829394;,
  -0.249287;-0.284124;0.925813;,
  -0.926858;-0.282600;-0.247128;,
  -0.831658;-0.282920;0.477808;,
  0.131709;-0.703883;0.697998;,
  0.130952;-0.991389;-0.000007;,
  0.131714;-0.703876;-0.698005;,
  0.132477;0.000005;0.991186;,
  0.132482;0.000003;-0.991185;,
  0.131711;0.703886;0.697995;,
  0.131714;0.703882;-0.697999;,
  0.130954;0.991388;0.000000;,
  -0.182942;-0.983124;-0.000005;,
  -0.183988;-0.697968;0.692090;,
  -0.183993;-0.697962;-0.692095;,
  -0.185048;0.000006;0.982729;,
  -0.185050;0.000004;-0.982729;,
  -0.183991;0.697968;0.692090;,
  -0.183991;0.697965;-0.692092;,
  -0.182946;0.983123;0.000002;;
  130;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;2,3,40,41;,
  4;2,41,42,4;,
  4;3,5,43,40;,
  4;4,42,44,6;,
  4;5,7,45,43;,
  4;6,44,46,8;,
  4;7,9,47,45;,
  4;8,46,47,9;,
  4;10,48,49,11;,
  4;10,11,3,2;,
  4;10,2,4,12;,
  4;10,12,50,48;,
  4;11,49,51,13;,
  4;11,13,5,3;,
  4;12,4,6,14;,
  4;12,14,52,50;,
  4;13,51,53,15;,
  4;13,15,7,5;,
  4;14,6,8,16;,
  4;14,16,54,52;,
  4;15,53,55,17;,
  4;15,17,9,7;,
  4;16,8,9,17;,
  4;16,17,55,54;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  3;19,19,19;,
  4;20,21,56,57;,
  4;20,57,58,22;,
  4;21,23,59,56;,
  4;22,58,60,24;,
  4;23,25,61,59;,
  4;24,60,62,26;,
  4;25,27,63,61;,
  4;26,62,63,27;,
  4;28,64,65,29;,
  4;28,29,21,20;,
  4;28,20,22,30;,
  4;28,30,66,64;,
  4;29,65,67,31;,
  4;29,31,23,21;,
  4;30,22,24,32;,
  4;30,32,68,66;,
  4;31,67,69,33;,
  4;31,33,25,23;,
  4;32,24,26,34;,
  4;32,34,70,68;,
  4;33,69,71,35;,
  4;33,35,27,25;,
  4;34,26,27,35;,
  4;34,35,71,70;,
  4;36,36,36,36;,
  4;37,37,37,37;,
  4;36,36,36,36;,
  4;1,1,1,1;,
  4;37,37,37,37;,
  4;0,0,0,0;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;36,36,36,36;,
  4;19,19,19,19;,
  4;37,37,37,37;,
  4;18,18,18,18;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;36,36,36,36;,
  4;1,1,1,1;,
  4;37,37,37,37;,
  4;0,0,0,0;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;36,36,36,36;,
  4;1,1,1,1;,
  4;37,37,37,37;,
  4;0,0,0,0;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;36,36,36,36;,
  4;1,1,1,1;,
  4;37,37,37,37;,
  4;0,0,0,0;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;36,36,36,36;,
  4;1,1,1,1;,
  4;37,37,37,37;,
  4;0,0,0,0;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;36,36,36,36;,
  4;38,38,38,38;,
  4;37,37,37,37;,
  4;18,18,18,18;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;36,36,36,36;,
  4;39,39,39,39;,
  4;37,37,37,37;,
  4;18,18,18,18;,
  4;1,1,1,1;,
  4;0,0,0,0;;
 }
 MeshTextureCoords {
  268;
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  0.000000;0.973110;,
  0.125000;0.973110;,
  1.000000;0.973110;,
  0.875000;0.973110;,
  0.250000;0.973110;,
  0.750000;0.973110;,
  0.375000;0.973110;,
  0.625000;0.973110;,
  0.500000;0.973110;,
  0.000000;0.018670;,
  0.125000;0.018670;,
  1.000000;0.018670;,
  0.875000;0.018670;,
  0.250000;0.018670;,
  0.750000;0.018670;,
  0.375000;0.018670;,
  0.625000;0.018670;,
  0.500000;0.018670;,
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  0.000000;0.973110;,
  0.125000;0.973110;,
  1.000000;0.973110;,
  0.875000;0.973110;,
  0.250000;0.973110;,
  0.750000;0.973110;,
  0.375000;0.973110;,
  0.625000;0.973110;,
  0.500000;0.973110;,
  0.000000;0.018670;,
  0.125000;0.018670;,
  1.000000;0.018670;,
  0.875000;0.018670;,
  0.250000;0.018670;,
  0.750000;0.018670;,
  0.375000;0.018670;,
  0.625000;0.018670;,
  0.500000;0.018670;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}