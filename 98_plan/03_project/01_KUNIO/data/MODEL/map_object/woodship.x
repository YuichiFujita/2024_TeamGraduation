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
 72;
 78.57847;2.86061;-41.50000;,
 78.57847;16.95646;-41.50000;,
 158.36229;36.16741;0.00000;,
 123.23664;5.98028;0.00000;,
 78.57847;-19.83259;-0.00000;,
 63.70064;-11.61773;-35.32983;,
 78.57847;16.95646;41.50000;,
 78.57847;2.86061;41.50000;,
 65.27413;-11.61773;32.42104;,
 -121.63771;2.86061;-41.50000;,
 -121.63771;-11.61773;-41.50000;,
 -121.63771;-19.83259;0.00000;,
 -121.63771;-1.24681;0.00000;,
 -121.63771;14.93698;0.00000;,
 -121.63771;16.95646;-36.63495;,
 -121.63771;-11.61773;41.50000;,
 -121.63771;2.86061;41.50000;,
 -121.63771;16.95646;36.63495;,
 27.56993;-5.55185;-0.00000;,
 -0.19709;16.95646;41.50000;,
 158.36229;36.16741;0.00000;,
 78.57847;16.95646;-41.50000;,
 -0.19709;16.95646;-41.50000;,
 -0.19709;16.95646;41.50000;,
 -0.19709;2.86061;41.50000;,
 78.57847;16.95646;41.50000;,
 -0.19709;16.95646;-41.50000;,
 78.57847;16.95646;-41.50000;,
 78.57847;2.86061;-41.50000;,
 -0.19709;2.86061;-41.50000;,
 -6.84926;-11.61773;36.96052;,
 65.27413;-11.61773;32.42104;,
 63.70064;-11.61773;-35.32983;,
 -7.63599;-11.61773;-38.41490;,
 -6.84926;-11.61773;36.96052;,
 -0.19709;-19.83259;0.00000;,
 78.57847;-19.83259;-0.00000;,
 -7.63599;-11.61773;-38.41490;,
 63.70064;-11.61773;-35.32983;,
 -57.13306;16.95646;41.50000;,
 -43.24956;-2.73687;0.00000;,
 -57.13306;16.95646;41.50000;,
 -57.13306;2.86061;41.50000;,
 -57.13306;16.95646;-41.50000;,
 -60.45913;-11.61773;39.23025;,
 -57.13306;16.95646;-41.50000;,
 -57.13306;2.86061;-41.50000;,
 -60.45913;-11.61773;39.23025;,
 -57.13306;-19.83259;0.00000;,
 -60.85253;-11.61773;-39.95744;,
 -60.85253;-11.61773;-39.95744;,
 -85.60105;16.95646;-41.50000;,
 -121.63771;16.95646;-36.63495;,
 -121.63771;14.93698;0.00000;,
 -78.65928;-2.36492;0.00000;,
 -85.60105;16.95646;-41.50000;,
 -85.60105;2.86061;-41.50000;,
 -121.63771;2.86061;-41.50000;,
 -121.63771;16.95646;-36.63495;,
 -85.60105;16.95646;41.50000;,
 -87.46079;-11.61773;-40.72871;,
 -121.63771;-11.61773;-41.50000;,
 -85.60105;16.95646;41.50000;,
 -121.63771;16.95646;36.63495;,
 -85.60105;2.86061;41.50000;,
 -87.46079;-11.61773;-40.72871;,
 -85.60105;-19.83259;0.00000;,
 -121.63771;-19.83259;0.00000;,
 -121.63771;-11.61773;-41.50000;,
 -121.63771;-11.61773;41.50000;,
 -87.26406;-11.61773;40.36511;,
 -87.26406;-11.61773;40.36511;;
 
 40;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;3,2,6,7;,
 4;3,7,8,4;,
 4;9,10,11,12;,
 4;9,12,13,14;,
 4;12,11,15,16;,
 4;12,16,17,13;,
 4;18,19,6,20;,
 4;18,20,21,22;,
 4;23,24,7,25;,
 4;26,27,28,29;,
 4;24,30,31,7;,
 4;29,28,32,33;,
 4;34,35,36,8;,
 4;37,38,36,35;,
 4;39,19,18,40;,
 4;41,42,24,23;,
 4;40,18,22,43;,
 4;42,44,30,24;,
 4;45,26,29,46;,
 4;47,48,35,34;,
 4;46,29,33,49;,
 4;48,50,37,35;,
 4;51,52,53,54;,
 4;51,54,40,43;,
 4;55,45,46,56;,
 4;55,56,57,58;,
 4;54,53,17,59;,
 4;54,59,39,40;,
 4;56,46,49,60;,
 4;56,60,61,57;,
 4;62,63,16,64;,
 4;62,64,42,41;,
 4;65,50,48,66;,
 4;65,66,67,68;,
 4;64,16,69,70;,
 4;64,70,44,42;,
 4;66,48,47,71;,
 4;66,71,15,67;;
 
 MeshMaterialList {
  1;
  40;
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
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\wood_ship.jpg";
   }
  }
 }
 MeshNormals {
  59;
  -1.000000;0.000000;0.000000;,
  0.199348;-0.775000;-0.599695;,
  0.327715;-0.158458;-0.931394;,
  0.327715;-0.158458;0.931394;,
  0.194763;-0.797338;0.571244;,
  0.610463;-0.295173;-0.734988;,
  0.278003;-0.960570;0.004472;,
  0.317010;-0.382473;-0.867882;,
  0.580074;-0.550475;-0.600410;,
  0.316225;-0.415719;0.852748;,
  -0.074479;0.997223;0.000000;,
  -0.068805;0.921258;-0.382818;,
  -0.068805;0.921258;0.382818;,
  0.012935;-0.168553;0.985608;,
  0.008959;-0.119205;-0.992829;,
  0.017778;-0.747846;0.663634;,
  0.012701;-0.716751;-0.697214;,
  0.004800;-0.999976;0.005033;,
  0.011205;0.900915;-0.433852;,
  0.012437;0.999923;0.000000;,
  0.010554;-0.088230;0.996044;,
  0.011205;0.900915;0.433852;,
  0.021042;-0.175906;0.984182;,
  0.007237;-0.060752;-0.998127;,
  0.003672;-0.999991;0.002169;,
  0.014452;-0.121325;-0.992508;,
  0.095352;0.943078;0.318610;,
  0.100595;0.994928;-0.000000;,
  -0.009683;0.014153;-0.999853;,
  0.095352;0.943078;-0.318610;,
  0.012882;-0.054047;-0.998455;,
  -0.006714;0.001585;0.999976;,
  0.003187;-0.999995;0.000903;,
  0.018820;-0.079111;0.996688;,
  0.576013;-0.553018;0.601980;,
  0.610463;-0.295173;0.734988;,
  -0.154456;0.935568;-0.317578;,
  -0.162888;0.986645;0.000000;,
  -0.154456;0.935568;0.317578;,
  0.000000;-0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.004291;-0.978366;0.206839;,
  0.002898;-0.979259;-0.202592;,
  0.183976;0.960990;0.206520;,
  0.188029;0.982163;0.000000;,
  -0.032279;0.082523;-0.996066;,
  -0.026651;0.069262;-0.997242;,
  -0.064304;0.164397;-0.984296;,
  0.183976;0.960990;-0.206520;,
  0.011286;-0.026609;-0.999582;,
  -0.032279;0.082523;0.996066;,
  -0.064304;0.164397;0.984296;,
  -0.024040;0.063025;0.997722;,
  0.002530;-0.980237;-0.197810;,
  0.002752;-0.999996;0.000436;,
  0.002191;-0.980612;-0.195948;,
  0.016514;-0.039108;0.999098;,
  0.003717;-0.979874;0.199581;,
  0.003206;-0.980438;0.196803;;
  40;
  4;7,2,5,8;,
  4;7,8,6,1;,
  4;34,35,3,9;,
  4;34,9,4,6;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;10,11,36,37;,
  4;10,37,38,12;,
  4;39,13,9,3;,
  4;40,2,7,14;,
  4;13,15,4,9;,
  4;14,7,1,16;,
  4;15,17,6,4;,
  4;16,1,6,17;,
  4;18,11,10,19;,
  4;39,20,13,39;,
  4;19,10,12,21;,
  4;20,22,15,13;,
  4;40,40,14,23;,
  4;41,24,17,15;,
  4;23,14,16,25;,
  4;24,42,16,17;,
  4;26,43,44,27;,
  4;26,27,19,21;,
  4;45,40,23,28;,
  4;45,28,46,47;,
  4;27,44,48,29;,
  4;27,29,18,19;,
  4;28,23,25,30;,
  4;28,30,49,46;,
  4;50,51,52,31;,
  4;50,31,20,39;,
  4;53,42,24,32;,
  4;53,32,54,55;,
  4;31,52,56,33;,
  4;31,33,22,20;,
  4;32,24,41,57;,
  4;32,57,58,54;;
 }
 MeshTextureCoords {
  72;
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.500000;,
  1.000000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.000000;1.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.500000;1.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  1.000000;0.250000;,
  0.500000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.250000;,
  1.000000;0.250000;,
  0.500000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.250000;,
  0.000000;0.125000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.125000;,
  1.000000;0.125000;,
  0.500000;0.125000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.125000;,
  0.000000;0.125000;,
  0.000000;0.000000;,
  0.000000;0.125000;,
  0.000000;0.000000;,
  0.500000;0.125000;,
  1.000000;0.125000;,
  0.500000;0.125000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.125000;,
  0.000000;0.125000;;
 }
}