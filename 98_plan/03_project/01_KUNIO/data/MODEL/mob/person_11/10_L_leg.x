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
 102;
 -2.87970;-3.14113;3.81054;,
 1.10472;-8.82116;5.46971;,
 2.21911;-3.18166;3.84083;,
 -3.68701;-8.66927;5.27805;,
 -1.19536;-42.84003;-14.47424;,
 1.22073;-45.04492;-15.83056;,
 -3.63563;-44.48451;-15.86334;,
 1.10472;-8.82116;5.46971;,
 4.26131;-8.61608;0.71059;,
 3.59853;-2.54057;-0.19623;,
 -3.76440;-37.43119;0.40743;,
 -4.65644;-24.81624;1.07579;,
 -3.09570;-24.68611;-2.65259;,
 -0.49480;-37.38565;-3.33045;,
 2.61417;-43.15455;-3.53912;,
 -0.49480;-37.38565;-3.33045;,
 1.85416;-37.36487;0.44395;,
 2.80633;-42.26633;0.52085;,
 -1.32530;-37.41804;3.31132;,
 1.85416;-37.36487;0.44395;,
 2.84235;-24.79394;1.07963;,
 0.49315;-25.02347;5.10067;,
 -1.60710;-8.47129;-4.21300;,
 -5.26468;-8.54010;0.70091;,
 -4.61649;-2.43924;-0.20781;,
 -1.64671;-2.76942;-5.32497;,
 -5.41063;-14.15484;1.09147;,
 -3.11380;-14.05157;-3.27875;,
 0.49315;-25.02347;5.10067;,
 -3.83095;-24.96613;4.72669;,
 -1.32530;-37.41804;3.31132;,
 4.37841;-14.18298;1.09372;,
 0.68739;-24.70412;-2.88468;,
 1.23298;-14.09778;-3.55455;,
 -3.26492;-44.80453;-3.82693;,
 -4.11376;-41.32347;-4.00583;,
 -3.84572;-43.76154;-9.14792;,
 2.50858;-14.25389;5.69279;,
 -0.64133;-14.45620;6.34289;,
 -4.03829;-14.24938;5.74793;,
 -5.41063;-14.15484;1.09147;,
 -4.65644;-24.81624;1.07579;,
 2.42386;-8.49231;-3.68958;,
 1.88392;-2.76259;-4.70972;,
 -0.49480;-37.38565;-3.33045;,
 2.80633;-42.26633;0.52085;,
 -1.36808;-42.13133;4.35123;,
 -1.29436;-45.31371;3.76543;,
 1.63731;-45.57630;0.42844;,
 1.50045;-44.68834;-8.69465;,
 0.08074;-45.70104;-3.67748;,
 -3.26492;-44.80453;-3.82693;,
 -3.84572;-43.76154;-9.14792;,
 -1.98991;-45.70520;0.39242;,
 0.08074;-45.70104;-3.67748;,
 1.63731;-45.57630;0.42844;,
 1.50045;-44.68834;-8.69465;,
 2.61417;-43.15455;-3.53912;,
 -0.46837;-41.20141;-9.37186;,
 -0.49480;-37.38565;-3.33045;,
 -1.32530;-37.41804;3.31132;,
 -1.36808;-42.13133;4.35123;,
 -3.63563;-44.48451;-15.86334;,
 -1.19536;-42.84003;-14.47424;,
 -4.18415;-42.14361;0.33521;,
 -1.98991;-45.70520;0.39242;,
 -1.29436;-45.31371;3.76543;,
 -1.36808;-42.13133;4.35123;,
 -3.26492;-44.80453;-3.82693;,
 -4.18415;-42.14361;0.33521;,
 -1.32530;-37.41804;3.31132;,
 -3.76440;-37.43119;0.40743;,
 -4.18415;-42.14361;0.33521;,
 -1.36808;-42.13133;4.35123;,
 0.68739;-24.70412;-2.88468;,
 -0.49480;-37.38565;-3.33045;,
 1.22073;-45.04492;-15.83056;,
 -2.27163;-44.70235;-20.17686;,
 -3.63563;-44.48451;-15.86334;,
 -1.29436;-45.31371;3.76543;,
 2.61417;-43.15455;-3.53912;,
 1.22073;-45.04492;-15.83056;,
 -3.63563;-44.48451;-15.86334;,
 -2.27163;-44.70235;-20.17686;,
 -0.46837;-41.20141;-9.37186;,
 1.50045;-44.68834;-8.69465;,
 -3.76440;-37.43119;0.40743;,
 -1.60710;-8.47129;-4.21300;,
 -1.64671;-2.76942;-5.32497;,
 1.23298;-14.09778;-3.55455;,
 1.22073;-45.04492;-15.83056;,
 -1.19536;-42.84003;-14.47424;,
 -3.63563;-44.48451;-15.86334;,
 1.22073;-45.04492;-15.83056;,
 -5.26468;-8.54010;0.70091;,
 -4.61649;-2.43924;-0.20781;,
 0.73693;1.75118;-3.22672;,
 1.78200;1.87036;-0.16644;,
 -1.41786;1.74702;-3.60219;,
 -3.23181;1.93215;-0.17354;,
 -2.17322;1.48737;2.58450;,
 0.93866;1.46262;2.60301;;
 
 92;
 3;0,1,2;,
 3;1,0,3;,
 3;4,5,6;,
 3;2,7,8;,
 3;2,8,9;,
 3;10,11,12;,
 3;10,12,13;,
 3;14,15,16;,
 3;14,16,17;,
 3;18,19,20;,
 3;18,20,21;,
 3;22,23,24;,
 3;22,24,25;,
 3;12,11,26;,
 3;12,26,27;,
 3;28,29,30;,
 3;31,20,32;,
 3;31,32,33;,
 3;34,35,36;,
 3;21,37,38;,
 3;29,39,40;,
 3;29,40,41;,
 3;9,8,42;,
 3;9,42,43;,
 3;32,20,19;,
 3;32,19,44;,
 3;45,46,47;,
 3;45,47,48;,
 3;49,50,51;,
 3;49,51,52;,
 3;53,54,55;,
 3;56,57,54;,
 3;53,51,50;,
 3;58,36,35;,
 3;58,35,59;,
 3;60,61,17;,
 3;60,17,16;,
 3;62,36,58;,
 3;62,58,63;,
 3;64,65,66;,
 3;64,66,67;,
 3;68,65,64;,
 3;69,35,34;,
 3;70,71,72;,
 3;70,72,73;,
 3;12,74,75;,
 3;76,77,78;,
 3;79,53,55;,
 3;48,80,45;,
 3;81,82,83;,
 3;84,15,14;,
 3;84,14,85;,
 3;35,72,71;,
 3;35,71,59;,
 3;57,55,54;,
 3;30,29,41;,
 3;30,41,86;,
 3;29,28,38;,
 3;29,38,39;,
 3;20,31,37;,
 3;20,37,21;,
 3;43,42,87;,
 3;43,87,88;,
 3;74,12,27;,
 3;74,27,89;,
 3;84,85,90;,
 3;84,90,91;,
 3;52,92,93;,
 3;52,93,49;,
 3;1,3,39;,
 3;1,39,38;,
 3;37,1,38;,
 3;3,94,40;,
 3;3,40,39;,
 3;31,8,7;,
 3;31,7,37;,
 3;89,87,42;,
 3;33,42,8;,
 3;33,8,31;,
 3;23,22,27;,
 3;23,27,26;,
 3;22,89,27;,
 3;94,3,0;,
 3;94,0,95;,
 4;96,97,9,43;,
 4;98,96,43,25;,
 4;99,98,25,95;,
 4;99,97,96,98;,
 4;100,99,95,0;,
 4;101,100,0,2;,
 4;97,101,2,9;,
 4;99,100,101,97;;
 
 MeshMaterialList {
  9;
  92;
  0,
  0,
  4,
  0,
  0,
  0,
  0,
  4,
  4,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  4,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  0,
  0,
  4,
  4,
  4,
  4,
  4,
  4,
  0,
  0,
  0,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
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
  4,
  4,
  4,
  4,
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
   0.000000;0.800000;0.677647;1.000000;;
   0.000000;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.700000;0.708800;0.533600;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.661600;0.044000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.291765;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  87;
  -0.581159;0.344804;-0.737133;,
  0.902719;0.133571;-0.408972;,
  0.620873;0.242882;0.745336;,
  -0.012514;0.265134;0.964130;,
  0.976624;0.197479;-0.084901;,
  0.744814;0.271921;-0.609353;,
  -0.892371;0.152161;-0.424878;,
  0.693729;-0.195442;0.693211;,
  0.054502;0.853438;-0.518337;,
  0.600997;-0.026242;0.798820;,
  0.114717;0.993301;-0.013886;,
  0.676296;0.017403;0.736424;,
  -0.102896;0.915068;-0.389952;,
  0.458546;0.817277;-0.348990;,
  -0.930636;-0.171147;0.323460;,
  0.866692;-0.472554;0.159804;,
  -0.421270;-0.903016;0.084227;,
  0.960806;0.093011;-0.261153;,
  0.214009;-0.968747;-0.125418;,
  -0.941015;-0.334860;0.048576;,
  -0.991541;-0.129794;-0.000063;,
  0.856423;-0.044377;0.514364;,
  0.519985;-0.170843;0.836916;,
  -0.117354;-0.117484;0.986116;,
  -0.677906;-0.051686;-0.733329;,
  -0.992377;-0.105505;0.063690;,
  0.834814;-0.153448;0.528716;,
  -0.976594;0.128596;-0.172417;,
  -0.970481;-0.233558;0.060151;,
  -0.576242;0.767851;-0.279912;,
  -0.865344;0.076040;-0.495377;,
  0.856194;-0.077211;-0.510853;,
  0.038107;-0.995411;-0.087775;,
  -0.999398;-0.013194;-0.032089;,
  -0.988814;0.099473;-0.111144;,
  -0.958923;0.019351;0.283004;,
  -0.033847;0.174066;0.984152;,
  0.694785;-0.016920;0.719019;,
  0.999726;0.004559;0.022960;,
  0.998229;-0.010717;-0.058513;,
  0.364744;0.174773;0.914558;,
  -0.003938;-0.005168;0.999979;,
  -0.521820;-0.035657;-0.852310;,
  0.925347;-0.044578;-0.376491;,
  -0.686865;-0.037453;-0.725820;,
  0.492868;-0.107720;-0.863410;,
  -0.003706;0.256582;0.966515;,
  -0.877895;0.260554;0.401761;,
  0.929128;0.369750;0.002427;,
  0.585410;0.392410;-0.709443;,
  -0.957318;0.285675;-0.043946;,
  0.160502;0.344666;-0.924903;,
  0.847035;0.287351;-0.447169;,
  0.669272;0.025298;0.742587;,
  -0.087144;-0.129805;0.987703;,
  0.874581;-0.085156;-0.477343;,
  -0.965442;-0.110419;0.236070;,
  -0.215403;-0.958464;-0.186943;,
  -0.149057;-0.988816;-0.005070;,
  0.034822;-0.997426;0.062673;,
  0.706192;-0.705690;0.057391;,
  -0.596312;0.765527;-0.241622;,
  -0.601013;0.671319;-0.433720;,
  -0.605183;0.789276;-0.103906;,
  -0.849682;-0.326687;0.413903;,
  -0.818764;0.004599;0.574112;,
  -0.847608;-0.028908;0.529835;,
  -0.765609;-0.014848;0.643135;,
  -0.062149;-0.025916;-0.997730;,
  0.034231;-0.993534;0.108256;,
  -0.135004;-0.989169;0.057614;,
  -0.114829;-0.991668;0.058394;,
  -0.114717;-0.993301;0.013886;,
  0.794970;0.551633;-0.252436;,
  0.870658;0.455576;-0.185488;,
  -0.125131;0.063284;0.990120;,
  0.147651;-0.174992;-0.973436;,
  0.127483;-0.106901;-0.986063;,
  0.141464;-0.133932;-0.980841;,
  0.170945;0.081528;-0.981902;,
  -0.942756;0.117529;0.312087;,
  -0.899829;0.213002;0.380707;,
  -0.839923;0.252063;-0.480618;,
  0.009586;0.998574;0.052515;,
  0.009606;0.998862;-0.046718;,
  0.875668;0.346933;0.335921;,
  0.009472;0.988453;0.151231;;
  92;
  3;3,40,2;,
  3;40,3,36;,
  3;8,13,12;,
  3;2,40,38;,
  3;2,38,4;,
  3;6,25,24;,
  3;6,24,0;,
  3;17,52,1;,
  3;17,1,21;,
  3;9,53,26;,
  3;9,26,22;,
  3;44,34,27;,
  3;44,27,30;,
  3;24,25,33;,
  3;24,33,42;,
  3;54,23,9;,
  3;39,55,31;,
  3;39,31,45;,
  3;19,20,28;,
  3;22,37,41;,
  3;56,35,33;,
  3;56,33,25;,
  3;4,38,43;,
  3;4,43,5;,
  3;31,55,1;,
  3;31,1,52;,
  3;21,11,7;,
  3;21,7,15;,
  3;32,18,57;,
  3;32,57,58;,
  3;16,18,59;,
  3;32,60,18;,
  3;16,57,18;,
  3;29,61,62;,
  3;29,62,0;,
  3;9,11,21;,
  3;9,21,53;,
  3;63,61,29;,
  3;12,29,8;,
  3;14,16,64;,
  3;14,64,65;,
  3;19,16,14;,
  3;14,20,19;,
  3;66,67,14;,
  3;66,14,65;,
  3;24,68,0;,
  3;13,10,12;,
  3;69,16,59;,
  3;15,17,21;,
  3;70,71,72;,
  3;73,52,17;,
  3;73,17,74;,
  3;20,14,6;,
  3;62,6,0;,
  3;17,15,18;,
  3;66,56,25;,
  3;66,25,67;,
  3;23,22,41;,
  3;23,41,75;,
  3;26,39,37;,
  3;26,37,22;,
  3;76,77,78;,
  3;76,78,79;,
  3;68,24,42;,
  3;68,42,45;,
  3;73,74,13;,
  3;73,13,8;,
  3;58,71,70;,
  3;58,70,32;,
  3;40,36,75;,
  3;40,75,41;,
  3;37,40,41;,
  3;80,34,33;,
  3;80,33,35;,
  3;39,38,40;,
  3;39,40,37;,
  3;45,78,77;,
  3;45,43,38;,
  3;45,38,39;,
  3;34,44,42;,
  3;34,42,33;,
  3;44,45,42;,
  3;34,80,81;,
  3;34,81,27;,
  4;49,48,4,5;,
  4;51,49,5,79;,
  4;50,82,30,27;,
  4;83,83,84,84;,
  4;47,50,27,81;,
  4;46,46,3,2;,
  4;48,85,2,4;,
  4;83,86,86,83;;
 }
 MeshTextureCoords {
  102;
  0.075280;0.827530;,
  0.092390;0.857490;,
  0.104800;0.825220;,
  0.076050;0.857970;,
  0.096670;0.196580;,
  0.102320;0.182780;,
  0.092080;0.182570;,
  0.106010;0.855570;,
  0.128860;0.851620;,
  0.128560;0.823960;,
  0.178700;0.991910;,
  0.194380;0.936900;,
  0.174280;0.932510;,
  0.162630;0.988480;,
  0.136220;0.265840;,
  0.115440;0.279330;,
  0.137230;0.299990;,
  0.149740;0.289050;,
  0.115550;0.984290;,
  0.129790;0.984310;,
  0.130400;0.928690;,
  0.112410;0.929280;,
  0.182260;0.849430;,
  0.208380;0.854210;,
  0.208110;0.826870;,
  0.182900;0.824320;,
  0.205760;0.883380;,
  0.180810;0.878720;,
  0.098810;0.931730;,
  0.084490;0.933530;,
  0.098970;0.987160;,
  0.129580;0.877980;,
  0.149290;0.929220;,
  0.151130;0.876680;,
  0.054130;0.262880;,
  0.066760;0.268420;,
  0.071120;0.231430;,
  0.107570;0.880580;,
  0.093450;0.883240;,
  0.077820;0.883910;,
  0.053540;0.885610;,
  0.066680;0.938410;,
  0.152690;0.848560;,
  0.155060;0.822660;,
  0.145920;0.985740;,
  0.601510;0.601740;,
  0.625420;0.630850;,
  0.629170;0.613000;,
  0.617120;0.593750;,
  0.634570;0.513360;,
  0.638930;0.558070;,
  0.653340;0.557370;,
  0.654730;0.511340;,
  0.640870;0.590860;,
  0.626600;0.559370;,
  0.629250;0.591500;,
  0.607440;0.516680;,
  0.611310;0.561660;,
  0.087030;0.233700;,
  0.085800;0.277230;,
  0.150450;0.317100;,
  0.164680;0.309700;,
  0.076220;0.197720;,
  0.086580;0.196970;,
  0.682460;0.596670;,
  0.664360;0.590650;,
  0.654350;0.612140;,
  0.661030;0.628870;,
  0.670450;0.557180;,
  0.041710;0.286080;,
  0.052070;0.318980;,
  0.063820;0.301450;,
  0.052090;0.293150;,
  0.038000;0.313280;,
  0.160430;0.930650;,
  0.154550;0.987160;,
  0.564240;0.039400;,
  0.555030;0.028640;,
  0.553630;0.042420;,
  0.641700;0.613930;,
  0.595970;0.564100;,
  0.544170;0.082850;,
  0.553070;0.082880;,
  0.551760;0.073190;,
  0.109730;0.232590;,
  0.125470;0.231420;,
  0.086040;0.991910;,
  0.167510;0.848340;,
  0.169850;0.822570;,
  0.165570;0.877290;,
  0.116520;0.197260;,
  0.106600;0.196580;,
  0.658210;0.458060;,
  0.628380;0.459980;,
  0.052130;0.855800;,
  0.052090;0.828450;,
  0.155060;0.822660;,
  0.128560;0.823960;,
  0.182900;0.824320;,
  0.052090;0.828450;,
  0.075280;0.827530;,
  0.104800;0.825220;;
 }
}