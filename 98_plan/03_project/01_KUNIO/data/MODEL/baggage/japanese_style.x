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
 176;
 0.00000;93.61314;-0.61828;,
 22.04956;89.22888;-14.73745;,
 0.00000;89.22888;-23.26369;,
 0.00000;93.61314;-0.61828;,
 27.49533;89.22888;4.42078;,
 0.00000;93.61314;-0.61828;,
 12.23658;89.22888;19.78450;,
 0.00000;93.61314;-0.61828;,
 -12.23658;89.22888;19.78450;,
 0.00000;93.61314;-0.61828;,
 -27.49533;89.22888;4.42078;,
 0.00000;93.61314;-0.61828;,
 -22.04956;89.22888;-14.73745;,
 0.00000;93.61314;-0.61828;,
 0.00000;89.22888;-23.26369;,
 39.73196;71.49864;-26.06013;,
 0.00000;71.49864;-41.42391;,
 49.54489;71.49864;8.46183;,
 22.04956;71.49864;36.14630;,
 -22.04956;71.49864;36.14630;,
 -49.54489;71.49864;8.46183;,
 -39.73190;71.49864;-26.06019;,
 0.00000;71.49864;-41.42391;,
 49.54489;53.74707;-32.34381;,
 0.00000;53.74707;-51.50204;,
 61.78146;53.74707;10.70439;,
 27.49533;53.74707;45.22642;,
 -27.49539;53.74707;45.22642;,
 -61.78153;53.74707;10.70439;,
 -49.54489;53.74707;-32.34381;,
 0.00000;53.74707;-51.50204;,
 49.54489;34.04434;-32.34381;,
 0.00000;34.04434;-51.50204;,
 61.78146;34.04434;10.70439;,
 27.49533;34.04434;45.22642;,
 -27.49539;34.04434;45.22642;,
 -61.78153;34.04434;10.70439;,
 -49.54489;34.04434;-32.34381;,
 0.00000;34.04434;-51.50204;,
 46.84030;17.78380;-30.25042;,
 0.00000;17.78380;-48.36286;,
 58.40888;17.78380;10.44784;,
 25.99441;17.78380;43.08525;,
 -25.99441;17.78380;43.08525;,
 -58.40888;17.78380;10.44777;,
 -46.84030;17.78380;-30.25042;,
 0.00000;17.78380;-48.36286;,
 25.99441;3.30155;-16.90201;,
 0.00000;3.30155;-26.95361;,
 32.41446;3.30155;5.68379;,
 14.42578;3.30155;23.79624;,
 -14.42584;3.30155;23.79624;,
 -32.41446;3.30155;5.68379;,
 -25.99441;3.30155;-16.90201;,
 0.00000;3.30155;-26.95361;,
 0.00000;-1.86712;-0.25682;,
 0.00000;-1.86712;-0.25682;,
 0.00000;-1.86712;-0.25682;,
 0.00000;-1.86712;-0.25682;,
 0.00000;-1.86712;-0.25682;,
 0.00000;-1.86712;-0.25682;,
 0.00000;-1.86712;-0.25682;,
 0.00000;103.26414;-2.87475;,
 3.36349;102.51768;-4.34902;,
 0.00000;102.51768;-5.23933;,
 0.00000;103.26414;-2.87475;,
 4.19426;102.51768;-2.34858;,
 0.00000;103.26414;-2.87475;,
 1.86661;102.51768;-0.74432;,
 0.00000;103.26414;-2.87475;,
 -1.86661;102.51768;-0.74432;,
 0.00000;103.26414;-2.87475;,
 -4.19426;102.51768;-2.34858;,
 0.00000;103.26414;-2.87475;,
 -3.36355;102.51768;-4.34902;,
 0.00000;103.26414;-2.87475;,
 0.00000;102.51768;-5.23933;,
 6.06086;99.49946;-5.53131;,
 0.00000;99.49946;-7.13557;,
 7.55775;99.49946;-1.92660;,
 3.36349;99.49946;0.96415;,
 -3.36355;99.49946;0.96415;,
 -7.55775;99.49946;-1.92660;,
 -6.06086;99.49946;-5.53131;,
 0.00000;99.49946;-7.13557;,
 7.55775;96.47749;-6.18742;,
 0.00000;96.47749;-8.18785;,
 9.42435;96.47749;-1.69247;,
 4.19426;96.47749;1.91224;,
 -4.19426;96.47749;1.91224;,
 -9.42441;96.47749;-1.69247;,
 -7.55775;96.47749;-6.18742;,
 0.00000;96.47749;-8.18785;,
 7.55775;93.12342;-6.18742;,
 0.00000;93.12342;-8.18785;,
 9.42435;93.12342;-1.69247;,
 4.19426;93.12342;1.91224;,
 -4.19426;93.12342;1.91224;,
 -9.42441;93.12342;-1.69247;,
 -7.55775;93.12342;-6.18742;,
 0.00000;93.12342;-8.18785;,
 7.14519;90.35533;-5.96882;,
 0.00000;90.35533;-7.86013;,
 8.90988;90.35533;-1.71925;,
 3.96526;90.35533;1.68870;,
 -3.96526;90.35533;1.68870;,
 -8.90994;90.35533;-1.71925;,
 -7.14519;90.35533;-5.96882;,
 0.00000;90.35533;-7.86013;,
 3.96526;87.88995;-4.57502;,
 0.00000;87.88995;-5.62458;,
 4.94462;87.88995;-2.21670;,
 2.20057;87.88995;-0.32546;,
 -2.20057;87.88995;-0.32546;,
 -4.94462;87.88995;-2.21670;,
 -3.96526;87.88995;-4.57502;,
 0.00000;87.88995;-5.62458;,
 0.00000;87.01004;-2.83699;,
 0.00000;87.01004;-2.83699;,
 0.00000;87.01004;-2.83699;,
 0.00000;87.01004;-2.83699;,
 0.00000;87.01004;-2.83699;,
 0.00000;87.01004;-2.83699;,
 0.00000;87.01004;-2.83699;,
 3.73639;95.51022;-6.44553;,
 22.81168;100.62143;-2.73370;,
 4.81663;91.47879;-5.35841;,
 22.81168;100.62143;-2.73370;,
 5.26403;89.80888;-2.73370;,
 22.81168;100.62143;-2.73370;,
 4.81663;91.47879;-0.10900;,
 22.81168;100.62143;-2.73370;,
 3.73639;95.51022;0.97819;,
 22.81168;100.62143;-2.73370;,
 2.65616;99.54171;-0.10900;,
 22.81168;100.62143;-2.73370;,
 2.20870;101.21163;-2.73370;,
 22.81168;100.62143;-2.73370;,
 2.65616;99.54171;-5.35834;,
 22.81168;100.62143;-2.73370;,
 3.73639;95.51022;-6.44553;,
 3.73639;95.51022;-2.73370;,
 3.73639;95.51022;-6.44553;,
 4.81663;91.47879;-5.35841;,
 5.26403;89.80888;-2.73370;,
 4.81663;91.47879;-0.10900;,
 3.73639;95.51022;0.97819;,
 2.65616;99.54171;-0.10900;,
 2.20870;101.21163;-2.73370;,
 2.65616;99.54171;-5.35834;,
 -5.51083;95.44327;-6.11416;,
 -24.58625;100.55448;-2.40227;,
 -4.43066;99.47477;-5.02697;,
 -24.58625;100.55448;-2.40227;,
 -3.98320;101.14462;-2.40227;,
 -24.58625;100.55448;-2.40227;,
 -4.43066;99.47477;0.22243;,
 -24.58625;100.55448;-2.40227;,
 -5.51083;95.44327;1.30962;,
 -24.58625;100.55448;-2.40227;,
 -6.59113;91.41178;0.22243;,
 -24.58625;100.55448;-2.40227;,
 -7.03852;89.74193;-2.40227;,
 -24.58625;100.55448;-2.40227;,
 -6.59113;91.41178;-5.02697;,
 -24.58625;100.55448;-2.40227;,
 -5.51083;95.44327;-6.11416;,
 -5.51083;95.44327;-2.40227;,
 -5.51083;95.44327;-6.11416;,
 -4.43066;99.47477;-5.02697;,
 -3.98320;101.14462;-2.40227;,
 -4.43066;99.47477;0.22243;,
 -5.51083;95.44327;1.30962;,
 -6.59113;91.41178;0.22243;,
 -7.03852;89.74193;-2.40227;,
 -6.59113;91.41178;-5.02697;;
 
 130;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 4;2,1,15,16;,
 4;1,4,17,15;,
 4;4,6,18,17;,
 4;6,8,19,18;,
 4;8,10,20,19;,
 4;10,12,21,20;,
 4;12,14,22,21;,
 4;16,15,23,24;,
 4;15,17,25,23;,
 4;17,18,26,25;,
 4;18,19,27,26;,
 4;19,20,28,27;,
 4;20,21,29,28;,
 4;21,22,30,29;,
 4;24,23,31,32;,
 4;23,25,33,31;,
 4;25,26,34,33;,
 4;26,27,35,34;,
 4;27,28,36,35;,
 4;28,29,37,36;,
 4;29,30,38,37;,
 4;32,31,39,40;,
 4;31,33,41,39;,
 4;33,34,42,41;,
 4;34,35,43,42;,
 4;35,36,44,43;,
 4;36,37,45,44;,
 4;37,38,46,45;,
 4;40,39,47,48;,
 4;39,41,49,47;,
 4;41,42,50,49;,
 4;42,43,51,50;,
 4;43,44,52,51;,
 4;44,45,53,52;,
 4;45,46,54,53;,
 3;48,47,55;,
 3;47,49,56;,
 3;49,50,57;,
 3;50,51,58;,
 3;51,52,59;,
 3;52,53,60;,
 3;53,54,61;,
 3;62,63,64;,
 3;65,66,63;,
 3;67,68,66;,
 3;69,70,68;,
 3;71,72,70;,
 3;73,74,72;,
 3;75,76,74;,
 4;64,63,77,78;,
 4;63,66,79,77;,
 4;66,68,80,79;,
 4;68,70,81,80;,
 4;70,72,82,81;,
 4;72,74,83,82;,
 4;74,76,84,83;,
 4;78,77,85,86;,
 4;77,79,87,85;,
 4;79,80,88,87;,
 4;80,81,89,88;,
 4;81,82,90,89;,
 4;82,83,91,90;,
 4;83,84,92,91;,
 4;86,85,93,94;,
 4;85,87,95,93;,
 4;87,88,96,95;,
 4;88,89,97,96;,
 4;89,90,98,97;,
 4;90,91,99,98;,
 4;91,92,100,99;,
 4;94,93,101,102;,
 4;93,95,103,101;,
 4;95,96,104,103;,
 4;96,97,105,104;,
 4;97,98,106,105;,
 4;98,99,107,106;,
 4;99,100,108,107;,
 4;102,101,109,110;,
 4;101,103,111,109;,
 4;103,104,112,111;,
 4;104,105,113,112;,
 4;105,106,114,113;,
 4;106,107,115,114;,
 4;107,108,116,115;,
 3;110,109,117;,
 3;109,111,118;,
 3;111,112,119;,
 3;112,113,120;,
 3;113,114,121;,
 3;114,115,122;,
 3;115,116,123;,
 3;124,125,126;,
 3;126,127,128;,
 3;128,129,130;,
 3;130,131,132;,
 3;132,133,134;,
 3;134,135,136;,
 3;136,137,138;,
 3;138,139,140;,
 3;141,142,143;,
 3;141,143,144;,
 3;141,144,145;,
 3;141,145,146;,
 3;141,146,147;,
 3;141,147,148;,
 3;141,148,149;,
 3;141,149,142;,
 3;150,151,152;,
 3;152,153,154;,
 3;154,155,156;,
 3;156,157,158;,
 3;158,159,160;,
 3;160,161,162;,
 3;162,163,164;,
 3;164,165,166;,
 3;167,168,169;,
 3;167,169,170;,
 3;167,170,171;,
 3;167,171,172;,
 3;167,172,173;,
 3;167,173,174;,
 3;167,174,175;,
 3;167,175,168;;
 
 MeshMaterialList {
  1;
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
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\map_object\\japanese_style.png";
   }
  }
 }
 MeshNormals {
  139;
  -0.000000;1.000000;0.000000;,
  0.000000;0.888166;-0.459522;,
  0.302787;0.907534;-0.291035;,
  0.380793;0.918731;0.104544;,
  0.165911;0.894005;0.416207;,
  -0.165911;0.894005;0.416207;,
  -0.380793;0.918731;0.104544;,
  -0.302787;0.907534;-0.291035;,
  0.000000;0.611666;-0.791116;,
  0.547861;0.658701;-0.515715;,
  0.701510;0.687382;0.188121;,
  0.293460;0.625489;0.722942;,
  -0.293459;0.625489;0.722942;,
  -0.701509;0.687383;0.188120;,
  -0.547860;0.658701;-0.515715;,
  0.000000;0.257375;-0.966312;,
  0.704601;0.286956;-0.648994;,
  0.920889;0.306747;0.240563;,
  0.368026;0.265747;0.891030;,
  -0.368026;0.265747;0.891030;,
  -0.920888;0.306748;0.240562;,
  -0.704600;0.286956;-0.648995;,
  0.000000;-0.095322;-0.995447;,
  0.734699;-0.103353;-0.670474;,
  0.957430;-0.096235;-0.272152;,
  0.382740;-0.077856;0.920570;,
  -0.382740;-0.077858;0.920569;,
  -0.707342;-0.078411;0.702509;,
  -0.734699;-0.103353;-0.670474;,
  0.000000;-0.558497;-0.829507;,
  0.588357;-0.594001;-0.548634;,
  0.699504;-0.714584;-0.007990;,
  0.314974;-0.552956;0.771383;,
  -0.314975;-0.552956;0.771382;,
  -0.610332;-0.705107;0.360998;,
  -0.588357;-0.594001;-0.548634;,
  0.000000;-0.924558;-0.381042;,
  0.246529;-0.938964;-0.239937;,
  0.309091;-0.947133;0.086032;,
  0.135779;-0.928946;0.344417;,
  -0.135780;-0.928946;0.344417;,
  -0.309091;-0.947133;0.086031;,
  -0.246529;-0.938964;-0.239937;,
  -0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;-0.000003;,
  0.000001;0.793719;-0.608284;,
  0.312307;0.859995;-0.403576;,
  0.402678;0.903813;0.144817;,
  0.162349;0.812106;0.560469;,
  -0.162349;0.812107;0.560468;,
  -0.402680;0.903812;0.144819;,
  -0.312308;0.859995;-0.403573;,
  0.000000;0.433475;-0.901166;,
  0.529807;0.551019;-0.644734;,
  0.729084;0.640847;0.240316;,
  0.255899;0.463652;0.848259;,
  -0.255899;0.463653;0.848258;,
  -0.729084;0.640848;0.240316;,
  -0.529808;0.551019;-0.644734;,
  0.000000;0.167056;-0.985947;,
  0.634416;0.225645;-0.739325;,
  0.895315;0.245319;-0.371793;,
  0.289969;0.181167;0.939732;,
  -0.289967;0.181167;0.939733;,
  -0.556579;0.195136;0.807553;,
  -0.634412;0.225646;-0.739328;,
  0.000000;-0.058909;-0.998263;,
  0.655172;-0.078064;-0.751436;,
  0.920342;-0.083094;-0.382187;,
  0.296557;-0.050644;0.953671;,
  -0.296554;-0.050644;0.953672;,
  -0.566583;-0.056448;0.822069;,
  -0.655167;-0.078064;-0.751440;,
  0.000000;-0.416437;-0.909165;,
  0.548755;-0.508625;-0.663452;,
  0.729947;-0.683312;-0.016168;,
  0.260893;-0.428377;0.865117;,
  -0.260891;-0.428378;0.865118;,
  -0.506431;-0.451227;0.734793;,
  -0.548752;-0.508624;-0.663455;,
  0.000000;-0.843042;-0.537847;,
  0.261654;-0.899419;-0.350118;,
  0.334055;-0.934061;0.126240;,
  0.139350;-0.859356;0.492026;,
  -0.139349;-0.859356;0.492025;,
  -0.334053;-0.934062;0.126240;,
  -0.261653;-0.899419;-0.350119;,
  0.000000;-1.000000;-0.000002;,
  0.178427;0.047815;-0.982791;,
  0.363587;-0.487792;-0.793639;,
  0.443635;-0.719974;-0.533690;,
  0.363590;-0.487798;0.793635;,
  0.178432;0.047809;0.982790;,
  0.070981;0.604235;0.793638;,
  0.024216;0.845328;0.533698;,
  0.070979;0.604230;-0.793642;,
  0.299204;-0.237793;-0.924084;,
  -0.965926;-0.258819;-0.000000;,
  -0.178430;0.047811;-0.982790;,
  -0.070980;0.604242;-0.793633;,
  -0.024213;0.845339;-0.533681;,
  -0.070980;0.604242;0.793633;,
  -0.178431;0.047811;0.982790;,
  -0.363591;-0.487801;0.793632;,
  -0.443638;-0.719982;0.533677;,
  -0.363591;-0.487801;-0.793632;,
  -0.140222;0.355544;-0.924081;,
  0.965926;-0.258818;-0.000000;,
  0.707342;-0.078409;0.702509;,
  -0.957430;-0.096235;-0.272153;,
  0.700802;-0.156336;0.696014;,
  -0.944078;-0.191577;-0.268357;,
  0.556582;0.195136;0.807551;,
  -0.895311;0.245322;-0.371801;,
  0.566587;-0.056448;0.822066;,
  -0.920337;-0.083094;-0.382198;,
  0.563876;-0.112716;0.818131;,
  -0.782558;-0.531029;-0.324979;,
  0.443635;-0.719974;0.533690;,
  0.299207;-0.237797;0.924082;,
  0.024216;0.845322;-0.533707;,
  -0.965925;-0.258821;0.000000;,
  -0.965926;-0.258818;-0.000008;,
  -0.965928;-0.258813;0.000000;,
  -0.965926;-0.258818;0.000008;,
  -0.965926;-0.258819;0.000001;,
  -0.965926;-0.258820;-0.000000;,
  -0.965926;-0.258819;-0.000001;,
  -0.024213;0.845339;0.533681;,
  -0.140222;0.355544;0.924081;,
  -0.443638;-0.719982;-0.533677;,
  0.965926;-0.258819;-0.000000;,
  0.965929;-0.258808;-0.000007;,
  0.965928;-0.258812;-0.000000;,
  0.965929;-0.258808;0.000007;,
  0.965926;-0.258819;-0.000000;,
  0.965923;-0.258828;0.000009;,
  0.965925;-0.258823;-0.000000;,
  0.965923;-0.258828;-0.000009;;
  130;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,1,7;,
  4;1,2,9,8;,
  4;2,3,10,9;,
  4;3,4,11,10;,
  4;4,5,12,11;,
  4;5,6,13,12;,
  4;6,7,14,13;,
  4;7,1,8,14;,
  4;8,9,16,15;,
  4;9,10,17,16;,
  4;10,11,18,17;,
  4;11,12,19,18;,
  4;12,13,20,19;,
  4;13,14,21,20;,
  4;14,8,15,21;,
  4;15,16,23,22;,
  4;16,17,24,23;,
  4;17,18,25,108;,
  4;18,19,26,25;,
  4;19,20,27,26;,
  4;20,21,28,109;,
  4;21,15,22,28;,
  4;22,23,30,29;,
  4;23,24,31,30;,
  4;108,25,32,110;,
  4;25,26,33,32;,
  4;26,27,34,33;,
  4;109,28,35,111;,
  4;28,22,29,35;,
  4;29,30,37,36;,
  4;30,31,38,37;,
  4;31,32,39,38;,
  4;32,33,40,39;,
  4;33,34,41,40;,
  4;34,35,42,41;,
  4;35,29,36,42;,
  3;36,37,43;,
  3;37,38,43;,
  3;38,39,43;,
  3;39,40,43;,
  3;40,41,43;,
  3;41,42,43;,
  3;42,36,43;,
  3;44,46,45;,
  3;44,47,46;,
  3;44,48,47;,
  3;44,49,48;,
  3;44,50,49;,
  3;44,51,50;,
  3;44,45,51;,
  4;45,46,53,52;,
  4;46,47,54,53;,
  4;47,48,55,54;,
  4;48,49,56,55;,
  4;49,50,57,56;,
  4;50,51,58,57;,
  4;51,45,52,58;,
  4;52,53,60,59;,
  4;53,54,61,60;,
  4;54,55,62,112;,
  4;55,56,63,62;,
  4;56,57,64,63;,
  4;57,58,65,113;,
  4;58,52,59,65;,
  4;59,60,67,66;,
  4;60,61,68,67;,
  4;112,62,69,114;,
  4;62,63,70,69;,
  4;63,64,71,70;,
  4;113,65,72,115;,
  4;65,59,66,72;,
  4;66,67,74,73;,
  4;67,68,75,74;,
  4;114,69,76,116;,
  4;69,70,77,76;,
  4;70,71,78,77;,
  4;115,72,79,117;,
  4;72,66,73,79;,
  4;73,74,81,80;,
  4;74,75,82,81;,
  4;75,76,83,82;,
  4;76,77,84,83;,
  4;77,78,85,84;,
  4;117,79,86,85;,
  4;79,73,80,86;,
  3;80,81,87;,
  3;81,82,87;,
  3;82,83,87;,
  3;83,84,87;,
  3;84,85,87;,
  3;85,86,87;,
  3;86,80,87;,
  3;88,96,89;,
  3;89,96,90;,
  3;118,119,91;,
  3;91,119,92;,
  3;92,119,93;,
  3;93,94,94;,
  3;120,120,95;,
  3;95,96,88;,
  3;97,121,122;,
  3;97,122,123;,
  3;97,123,124;,
  3;97,124,121;,
  3;97,121,125;,
  3;97,125,126;,
  3;97,126,127;,
  3;97,127,121;,
  3;98,106,99;,
  3;99,106,100;,
  3;128,129,101;,
  3;101,129,102;,
  3;102,129,103;,
  3;103,104,104;,
  3;130,130,105;,
  3;105,106,98;,
  3;107,131,132;,
  3;107,132,133;,
  3;107,133,134;,
  3;107,134,135;,
  3;107,135,136;,
  3;107,136,137;,
  3;107,137,138;,
  3;107,138,131;;
 }
 MeshTextureCoords {
  176;
  0.071430;0.000000;,
  0.142860;0.142860;,
  0.000000;0.142860;,
  0.214290;0.000000;,
  0.285710;0.142860;,
  0.357140;0.000000;,
  0.428570;0.142860;,
  0.500000;0.000000;,
  0.571430;0.142860;,
  0.642860;0.000000;,
  0.714290;0.142860;,
  0.785710;0.000000;,
  0.857140;0.142860;,
  0.928570;0.000000;,
  1.000000;0.142860;,
  0.142860;0.285710;,
  0.000000;0.285710;,
  0.285710;0.285710;,
  0.428570;0.285710;,
  0.571430;0.285710;,
  0.714290;0.285710;,
  0.857140;0.285710;,
  1.000000;0.285710;,
  0.142860;0.428570;,
  0.000000;0.428570;,
  0.285710;0.428570;,
  0.428570;0.428570;,
  0.571430;0.428570;,
  0.714290;0.428570;,
  0.857140;0.428570;,
  1.000000;0.428570;,
  0.142860;0.571430;,
  0.000000;0.571430;,
  0.285710;0.571430;,
  0.428570;0.571430;,
  0.571430;0.571430;,
  0.714290;0.571430;,
  0.857140;0.571430;,
  1.000000;0.571430;,
  0.142860;0.714290;,
  0.000000;0.714290;,
  0.285710;0.714290;,
  0.428570;0.714290;,
  0.571430;0.714290;,
  0.714290;0.714290;,
  0.857140;0.714290;,
  1.000000;0.714290;,
  0.142860;0.857140;,
  0.000000;0.857140;,
  0.285710;0.857140;,
  0.428570;0.857140;,
  0.571430;0.857140;,
  0.714290;0.857140;,
  0.857140;0.857140;,
  1.000000;0.857140;,
  0.071430;1.000000;,
  0.214290;1.000000;,
  0.357140;1.000000;,
  0.500000;1.000000;,
  0.642860;1.000000;,
  0.785710;1.000000;,
  0.928570;1.000000;,
  0.071430;0.000000;,
  0.142860;0.142860;,
  0.000000;0.142860;,
  0.214290;0.000000;,
  0.285710;0.142860;,
  0.357140;0.000000;,
  0.428570;0.142860;,
  0.500000;0.000000;,
  0.571430;0.142860;,
  0.642860;0.000000;,
  0.714290;0.142860;,
  0.785710;0.000000;,
  0.857140;0.142860;,
  0.928570;0.000000;,
  1.000000;0.142860;,
  0.142860;0.285710;,
  0.000000;0.285710;,
  0.285710;0.285710;,
  0.428570;0.285710;,
  0.571430;0.285710;,
  0.714290;0.285710;,
  0.857140;0.285710;,
  1.000000;0.285710;,
  0.142860;0.428570;,
  0.000000;0.428570;,
  0.285710;0.428570;,
  0.428570;0.428570;,
  0.571430;0.428570;,
  0.714290;0.428570;,
  0.857140;0.428570;,
  1.000000;0.428570;,
  0.142860;0.571430;,
  0.000000;0.571430;,
  0.285710;0.571430;,
  0.428570;0.571430;,
  0.571430;0.571430;,
  0.714290;0.571430;,
  0.857140;0.571430;,
  1.000000;0.571430;,
  0.142860;0.714290;,
  0.000000;0.714290;,
  0.285710;0.714290;,
  0.428570;0.714290;,
  0.571430;0.714290;,
  0.714290;0.714290;,
  0.857140;0.714290;,
  1.000000;0.714290;,
  0.142860;0.857140;,
  0.000000;0.857140;,
  0.285710;0.857140;,
  0.428570;0.857140;,
  0.571430;0.857140;,
  0.714290;0.857140;,
  0.857140;0.857140;,
  1.000000;0.857140;,
  0.071430;1.000000;,
  0.214290;1.000000;,
  0.357140;1.000000;,
  0.500000;1.000000;,
  0.642860;1.000000;,
  0.785710;1.000000;,
  0.928570;1.000000;,
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}