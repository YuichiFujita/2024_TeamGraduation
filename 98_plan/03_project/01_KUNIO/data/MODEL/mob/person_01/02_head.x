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
 252;
 -4.92249;39.60462;2.91516;,
 -5.62695;39.60462;-3.93867;,
 -10.07112;36.56040;0.25860;,
 -1.05246;39.60462;2.97036;,
 -0.79533;40.52379;-1.72272;,
 -5.62695;39.60462;-3.93867;,
 -0.79533;40.52379;-1.72272;,
 1.12392;39.60462;-6.82950;,
 -3.29037;36.56040;-11.49321;,
 -10.93308;20.09604;3.21213;,
 -12.02202;25.87764;3.61869;,
 -12.91764;24.43221;-1.72272;,
 2.10636;6.17928;-10.32405;,
 -4.61568;7.30695;-9.74304;,
 -0.49863;5.84139;-15.10773;,
 2.10636;6.17928;-10.32405;,
 -5.73288;0.50271;-7.36446;,
 -4.61568;7.30695;-9.74304;,
 -4.76457;36.56040;7.29135;,
 -4.76457;36.56040;7.29135;,
 -10.39098;16.92924;-0.90357;,
 -1.05246;39.60462;2.97036;,
 6.75489;38.08257;-0.73206;,
 6.75489;38.08257;-0.73206;,
 1.12392;39.60462;-6.82950;,
 5.51544;36.56040;-9.85155;,
 -3.29037;36.56040;-11.49321;,
 1.12392;39.60462;-6.82950;,
 2.41680;36.56040;7.29135;,
 6.75489;38.08257;-0.73206;,
 8.80029;16.92924;-0.90357;,
 11.32695;24.43221;-1.72272;,
 9.34239;20.09604;3.21213;,
 -5.73288;0.50271;-7.36446;,
 -7.87131;9.38187;-2.11245;,
 -7.15758;12.23370;4.66884;,
 -10.39098;16.92924;-0.90357;,
 -12.45669;31.65909;0.74472;,
 -12.91764;24.43221;-1.72272;,
 -10.39098;16.92924;-0.90357;,
 -12.91764;24.43221;-1.72272;,
 -10.58241;24.44430;-10.82739;,
 -6.91875;16.10343;-12.91290;,
 -12.45669;31.65909;0.74472;,
 -10.07112;36.56040;0.25860;,
 -8.89212;36.56040;-8.73246;,
 -10.32729;31.65909;-10.22079;,
 -12.91764;24.43221;-1.72272;,
 -10.58241;24.44430;-10.82739;,
 -5.62695;39.60462;-3.93867;,
 -6.91875;16.10343;-12.91290;,
 -6.21699;24.47922;-14.66148;,
 -0.82107;24.21057;-16.03701;,
 -0.81528;16.27242;-16.64886;,
 -8.97060;0.50271;2.55333;,
 -7.15758;12.23370;4.66884;,
 -7.87131;9.38187;-2.11245;,
 -7.87131;9.38187;-2.11245;,
 -7.15758;12.23370;4.66884;,
 -6.91875;16.10343;-12.91290;,
 -0.81528;16.27242;-16.64886;,
 -6.21699;24.47922;-14.66148;,
 -3.63918;31.65909;-14.11599;,
 -0.82107;24.21057;-16.03701;,
 -7.87131;9.38187;-2.11245;,
 -10.39098;16.92924;-0.90357;,
 -6.91875;16.10343;-12.91290;,
 -4.61568;7.30695;-9.74304;,
 -3.63918;31.65909;-14.11599;,
 -3.29037;36.56040;-11.49321;,
 -8.97060;0.50271;2.55333;,
 5.24406;2.84595;-5.15268;,
 2.10636;6.17928;-10.32405;,
 6.28062;9.38187;-2.11245;,
 1.29351;0.50271;-9.49452;,
 7.37991;0.50271;2.55333;,
 5.56689;12.23370;4.66884;,
 8.80029;16.92924;-0.90357;,
 8.99172;24.44430;-10.82739;,
 11.32695;24.43221;-1.72272;,
 8.80029;16.92924;-0.90357;,
 5.32806;16.10343;-12.91290;,
 10.86600;31.65909;0.74472;,
 8.73660;31.65909;-10.22079;,
 5.51544;36.56040;-9.85155;,
 6.75489;38.08257;-0.73206;,
 11.32695;24.43221;-1.72272;,
 8.99172;24.44430;-10.82739;,
 5.32806;16.10343;-12.91290;,
 -0.81528;16.27242;-16.64886;,
 -0.82107;24.21057;-16.03701;,
 4.62630;24.47922;-14.66148;,
 7.37991;0.50271;2.55333;,
 6.28062;9.38187;-2.11245;,
 5.56689;12.23370;4.66884;,
 5.56689;12.23370;4.66884;,
 6.28062;9.38187;-2.11245;,
 4.37631;31.65909;-13.36455;,
 -3.63918;31.65909;-14.11599;,
 2.10636;6.17928;-10.32405;,
 -0.49863;5.84139;-15.10773;,
 -0.81528;16.27242;-16.64886;,
 5.32806;16.10343;-12.91290;,
 4.62630;24.47922;-14.66148;,
 -0.82107;24.21057;-16.03701;,
 6.28062;9.38187;-2.11245;,
 2.10636;6.17928;-10.32405;,
 5.32806;16.10343;-12.91290;,
 8.80029;16.92924;-0.90357;,
 10.86600;31.65909;0.74472;,
 -0.07878;0.50271;6.39978;,
 7.37991;0.50271;2.55333;,
 -0.79560;12.25209;7.11756;,
 -0.07878;0.50271;6.39978;,
 -0.79560;12.25209;7.11756;,
 -8.97060;0.50271;2.55333;,
 -5.19093;37.26498;7.85448;,
 -1.03296;41.42106;3.98082;,
 -0.42744;37.15209;10.88475;,
 12.34131;24.33399;-1.52604;,
 10.29213;19.83798;3.54048;,
 11.40408;25.80672;3.95175;,
 2.84220;37.32438;7.78311;,
 2.41680;36.56040;7.29135;,
 -8.02962;32.02002;10.60299;,
 -10.19700;25.84395;9.97863;,
 -5.75700;25.87968;13.37883;,
 -7.72026;11.80887;5.35257;,
 -11.79762;19.83798;3.73365;,
 -7.37040;19.79289;10.82256;,
 1.65282;32.03067;12.65109;,
 -12.92034;25.84887;4.07802;,
 -13.27707;32.05815;1.19004;,
 -8.02962;32.02002;10.60299;,
 -5.19093;37.26498;7.85448;,
 6.21507;11.77956;5.25525;,
 7.88298;19.80645;9.12636;,
 11.74749;31.95975;1.00263;,
 12.34131;24.33399;-1.52604;,
 11.32695;24.43221;-1.72272;,
 8.63646;31.99947;9.09069;,
 8.56377;25.84227;10.02804;,
 2.84220;37.32438;7.78311;,
 -0.73599;11.71257;7.94427;,
 -0.79560;12.25209;7.11756;,
 1.31763;19.72896;13.09827;,
 -0.73599;11.71257;7.94427;,
 -0.79560;12.25209;7.11756;,
 1.31763;19.72896;13.09827;,
 -7.37040;19.79289;10.82256;,
 1.66992;25.88847;13.87785;,
 1.66992;25.88847;13.87785;,
 1.65282;32.03067;12.65109;,
 -31.71114;30.94053;6.29463;,
 -47.64471;24.52776;18.14460;,
 -50.78013;24.70173;3.45054;,
 -21.53850;30.94053;23.98626;,
 -36.99339;24.70173;34.69089;,
 -22.25601;24.35415;46.38480;,
 -6.11334;30.94053;31.89243;,
 -4.99167;24.70173;50.59890;,
 15.38154;30.94053;28.25475;,
 10.71066;24.70173;49.99176;,
 26.96226;24.52776;43.59966;,
 38.80563;24.70173;33.53478;,
 29.46267;30.94053;14.34888;,
 48.12927;24.52776;18.14448;,
 32.75163;30.94053;-2.19186;,
 50.78013;24.70173;-3.53940;,
 25.06548;30.94053;-20.42853;,
 43.90320;24.52776;-26.78106;,
 33.83844;24.70173;-38.62443;,
 18.44826;24.52776;-47.94816;,
 9.74895;30.94053;-31.38825;,
 -3.23586;24.70173;-50.59890;,
 -6.11334;30.94053;-32.01477;,
 -26.47752;24.52776;-43.72206;,
 -21.53850;30.94053;-24.10875;,
 -38.32092;24.70173;-33.65727;,
 -48.00723;24.52776;-16.54452;,
 -31.08459;30.94053;-9.56775;,
 0.02232;46.29006;-0.47721;,
 -12.31413;40.68825;12.79473;,
 5.33775;40.68825;16.50036;,
 -2.80317;35.01297;31.04376;,
 16.67118;35.01297;26.74743;,
 29.29128;35.01297;11.97099;,
 16.96815;40.68825;-3.41286;,
 30.81570;35.01297;-7.40148;,
 20.66223;35.01297;-23.97024;,
 -16.07985;40.68825;-5.10570;,
 -29.46030;35.01297;10.90305;,
 -30.00375;35.01297;-7.93539;,
 -21.24570;35.01297;23.19348;,
 2.70906;35.01297;-31.40643;,
 1.56897;40.68825;-17.99100;,
 -16.18632;35.01297;-26.86977;,
 0.02232;46.29006;-0.47721;,
 25.06548;30.94053;-20.42853;,
 9.74895;30.94053;-31.38825;,
 0.02232;46.29006;-0.47721;,
 32.75163;30.94053;-2.19186;,
 0.02232;46.29006;-0.47721;,
 29.46267;30.94053;14.34888;,
 0.02232;46.29006;-0.47721;,
 15.38154;30.94053;28.25475;,
 0.02232;46.29006;-0.47721;,
 -6.11334;30.94053;31.89243;,
 0.02232;46.29006;-0.47721;,
 -21.53850;30.94053;23.98626;,
 0.02232;46.29006;-0.47721;,
 -31.71114;30.94053;6.29463;,
 0.02232;46.29006;-0.47721;,
 -31.08459;30.94053;-9.56775;,
 0.02232;46.29006;-0.47721;,
 -21.53850;30.94053;-24.10875;,
 0.02232;46.29006;-0.47721;,
 -6.11334;30.94053;-32.01477;,
 0.02232;46.29006;-0.47721;,
 9.74895;30.94053;-31.38825;,
 0.02232;46.29006;-0.47721;,
 25.06548;30.94053;-20.42853;,
 0.02232;46.29006;-0.47721;,
 32.75163;30.94053;-2.19186;,
 0.02232;46.29006;-0.47721;,
 29.46267;30.94053;14.34888;,
 0.02232;46.29006;-0.47721;,
 15.38154;30.94053;28.25475;,
 0.02232;46.29006;-0.47721;,
 -6.11334;30.94053;31.89243;,
 0.02232;46.29006;-0.47721;,
 -21.53850;30.94053;23.98626;,
 0.02232;46.29006;-0.47721;,
 -31.71114;30.94053;6.29463;,
 0.02232;46.29006;-0.47721;,
 -31.08459;30.94053;-9.56775;,
 0.02232;46.29006;-0.47721;,
 -21.53850;30.94053;-24.10875;,
 0.02232;46.29006;-0.47721;,
 -6.11334;30.94053;-32.01477;,
 0.02232;46.29006;-0.47721;,
 9.74895;30.94053;-31.38825;,
 1.29351;0.50271;-9.49452;,
 2.10636;6.17928;-10.32405;,
 -5.73288;0.50271;-7.36446;,
 2.10636;6.17928;-10.32405;,
 1.29351;0.50271;-9.49452;,
 0.44814;-2.35119;-6.27825;,
 -3.73464;-2.35119;-5.01024;,
 4.07130;-2.35119;0.89373;,
 -5.66202;-2.35119;0.89373;,
 -0.36879;-2.35119;3.18351;;
 
 219;
 3;0,1,2;,
 3;3,4,0;,
 3;0,4,1;,
 3;5,6,7;,
 3;5,7,8;,
 3;9,10,11;,
 3;12,13,14;,
 3;15,16,17;,
 3;18,0,2;,
 3;3,0,19;,
 3;20,9,11;,
 3;21,22,4;,
 3;23,24,6;,
 3;25,26,27;,
 3;28,29,21;,
 3;30,31,32;,
 3;17,33,34;,
 3;35,9,36;,
 3;37,38,10;,
 3;39,40,41;,
 3;39,41,42;,
 3;43,44,45;,
 3;43,45,46;,
 3;47,43,46;,
 3;47,46,48;,
 3;44,49,45;,
 3;50,51,52;,
 3;50,52,53;,
 3;54,55,56;,
 3;36,57,58;,
 3;13,59,60;,
 3;13,60,14;,
 3;61,62,63;,
 3;64,65,66;,
 3;64,66,67;,
 3;48,46,68;,
 3;48,68,61;,
 3;45,5,69;,
 3;46,45,69;,
 3;46,69,68;,
 3;42,41,51;,
 3;18,2,37;,
 3;70,34,33;,
 3;71,72,73;,
 3;74,71,75;,
 3;76,77,32;,
 3;78,79,80;,
 3;78,80,81;,
 3;82,83,84;,
 3;82,84,85;,
 3;86,87,83;,
 3;86,83,82;,
 3;85,84,24;,
 3;88,89,90;,
 3;88,90,91;,
 3;92,93,94;,
 3;95,96,77;,
 3;97,98,26;,
 3;97,26,25;,
 3;99,100,101;,
 3;99,101,102;,
 3;103,104,98;,
 3;103,98,97;,
 3;105,106,107;,
 3;105,107,108;,
 3;87,103,97;,
 3;87,97,83;,
 3;83,97,25;,
 3;81,91,78;,
 3;109,29,28;,
 3;73,75,71;,
 4;110,111,94,112;,
 4;113,114,55,115;,
 4;116,117,3,19;,
 3;118,117,116;,
 4;119,120,32,31;,
 3;120,119,121;,
 4;117,122,123,3;,
 3;118,122,117;,
 3;124,125,126;,
 4;127,128,9,35;,
 3;127,129,128;,
 3;118,116,124;,
 3;118,124,130;,
 4;131,132,37,10;,
 3;133,132,131;,
 3;133,131,125;,
 4;132,134,18,37;,
 3;134,132,133;,
 3;126,125,129;,
 3;129,125,131;,
 4;128,131,10,9;,
 3;129,131,128;,
 4;120,135,76,32;,
 3;135,120,136;,
 4;137,138,139,109;,
 3;138,137,121;,
 3;121,137,140;,
 3;121,140,141;,
 4;142,137,109,28;,
 3;137,142,140;,
 3;136,120,121;,
 3;136,121,141;,
 4;135,143,144,76;,
 4;135,136,145,143;,
 4;146,127,35,147;,
 4;148,149,127,146;,
 4;126,149,148,150;,
 4;141,151,145,136;,
 4;124,126,150,130;,
 4;140,152,151,141;,
 4;140,122,118,152;,
 3;153,154,155;,
 3;154,153,156;,
 3;156,157,154;,
 3;156,158,157;,
 3;158,156,159;,
 3;159,160,158;,
 3;160,159,161;,
 3;161,162,160;,
 3;161,163,162;,
 3;161,164,163;,
 3;164,161,165;,
 3;165,166,164;,
 3;166,165,167;,
 3;167,168,166;,
 3;168,167,169;,
 3;169,170,168;,
 3;169,171,170;,
 3;169,172,171;,
 3;172,169,173;,
 3;173,174,172;,
 3;174,173,175;,
 3;175,176,174;,
 3;176,175,177;,
 3;177,178,176;,
 3;177,179,178;,
 3;179,177,180;,
 3;179,180,153;,
 3;153,155,179;,
 3;181,182,183;,
 3;184,183,182;,
 3;162,185,184;,
 3;183,184,185;,
 3;184,160,162;,
 3;185,162,163;,
 3;164,186,185;,
 3;183,185,186;,
 3;185,163,164;,
 3;186,187,183;,
 3;181,183,187;,
 3;186,164,166;,
 3;168,188,186;,
 3;187,186,188;,
 3;186,166,168;,
 3;170,189,188;,
 3;187,188,189;,
 3;188,168,170;,
 3;181,190,182;,
 3;191,155,154;,
 3;155,191,192;,
 3;190,192,191;,
 3;157,193,191;,
 3;191,154,157;,
 3;182,191,193;,
 3;191,182,190;,
 3;184,158,160;,
 3;158,184,193;,
 3;193,157,158;,
 3;182,193,184;,
 3;189,170,171;,
 3;172,194,189;,
 3;187,189,194;,
 3;194,195,187;,
 3;181,187,195;,
 3;189,171,172;,
 3;194,172,174;,
 3;174,196,194;,
 3;195,194,196;,
 3;196,190,195;,
 3;181,195,190;,
 3;196,174,176;,
 3;178,192,196;,
 3;190,196,192;,
 3;196,176,178;,
 3;192,178,179;,
 3;192,179,155;,
 3;197,198,199;,
 3;200,201,198;,
 3;202,203,201;,
 3;204,205,203;,
 3;206,207,205;,
 3;208,209,207;,
 3;210,211,209;,
 3;212,213,211;,
 3;214,215,213;,
 3;216,217,215;,
 3;218,219,217;,
 3;220,173,221;,
 3;222,221,223;,
 3;224,223,225;,
 3;226,225,227;,
 3;228,227,229;,
 3;230,229,231;,
 3;232,231,233;,
 3;234,233,235;,
 3;236,235,237;,
 3;238,237,239;,
 3;240,239,241;,
 3;242,243,71;,
 3;244,245,246;,
 4;247,248,16,74;,
 4;249,247,74,75;,
 3;248,247,249;,
 4;250,251,113,115;,
 4;248,250,115,16;,
 3;251,250,248;,
 4;251,249,75,113;,
 3;251,248,249;;
 
 MeshMaterialList {
  9;
  219;
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
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  0,
  6,
  0,
  6,
  6,
  6,
  6,
  6,
  6,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
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
  172;
  -0.370725;-0.499498;-0.782985;,
  -0.960785;-0.210589;-0.180400;,
  -0.289976;0.915315;0.279487;,
  -0.483492;0.667040;0.566827;,
  -0.724246;0.663903;0.186283;,
  -0.960149;0.275317;0.048104;,
  -0.993234;-0.111694;-0.031785;,
  -0.940081;-0.339452;-0.031934;,
  -0.686834;0.627672;-0.366453;,
  -0.780053;0.261266;-0.568558;,
  -0.898410;-0.193921;0.394023;,
  -0.974894;-0.146234;-0.167921;,
  -0.218309;0.296244;-0.929828;,
  -0.830735;-0.062803;-0.553114;,
  -0.724919;-0.245304;-0.643676;,
  -0.150825;-0.976939;0.151138;,
  -0.314831;0.931475;-0.182309;,
  -0.465444;0.042255;-0.884068;,
  -0.938153;-0.300878;0.171293;,
  -0.977315;-0.094623;0.189477;,
  0.759150;-0.466345;-0.454107;,
  0.922418;-0.284374;-0.261298;,
  0.874507;-0.181633;-0.449718;,
  0.019189;-0.454792;0.890391;,
  0.902631;-0.184732;0.388756;,
  0.017369;-0.083382;0.996366;,
  0.974227;-0.136019;-0.179947;,
  0.062716;0.997551;0.030963;,
  0.526721;0.843884;0.102101;,
  0.887867;0.459719;-0.018744;,
  0.539572;0.673147;-0.505702;,
  0.843235;0.267013;-0.466539;,
  0.154668;0.928927;0.336412;,
  0.527850;0.735406;0.424916;,
  -0.620731;-0.156139;-0.768318;,
  -0.004399;0.046155;-0.998925;,
  -0.136890;0.671792;-0.727981;,
  0.837307;-0.084728;-0.540128;,
  0.722046;-0.242449;-0.647972;,
  0.367611;0.324365;-0.871579;,
  0.130476;0.939690;-0.316162;,
  0.393619;0.079647;-0.915817;,
  0.923902;-0.352274;0.149363;,
  0.982338;-0.118326;-0.144948;,
  0.940081;-0.339452;-0.031934;,
  -0.780654;0.194892;-0.593799;,
  -0.777481;-0.030601;0.628161;,
  -0.628699;-0.311420;0.712570;,
  -0.490333;-0.403926;-0.772281;,
  -0.167593;-0.690670;-0.703483;,
  -0.392736;0.002524;0.919648;,
  -0.591492;0.352976;0.724945;,
  -0.489424;-0.215900;-0.844897;,
  -0.495131;-0.223393;-0.839607;,
  0.086567;-0.653042;-0.752358;,
  -0.005911;-0.865659;-0.500599;,
  0.182471;0.010591;0.983154;,
  0.747296;0.349026;0.565446;,
  0.732088;-0.031296;0.680490;,
  0.749026;-0.294969;0.593256;,
  0.021641;0.523095;-0.851999;,
  -0.012494;0.666360;0.745526;,
  0.167113;0.321517;0.932041;,
  0.123096;-0.379271;0.917061;,
  0.788325;0.153748;-0.595740;,
  0.200610;-0.573867;-0.793997;,
  0.941434;-0.060111;0.331795;,
  0.037523;-0.756515;-0.652899;,
  0.312888;-0.949677;-0.014630;,
  0.286448;-0.948875;-0.132600;,
  0.231810;-0.948051;-0.217860;,
  0.143820;-0.946438;-0.289085;,
  0.011151;-0.952480;-0.304397;,
  -0.069336;-0.954831;-0.288948;,
  -0.162368;-0.948026;-0.273650;,
  -0.231739;-0.950293;-0.207942;,
  -0.311472;-0.945801;-0.091899;,
  -0.313483;-0.947713;0.059727;,
  -0.273617;-0.950382;0.148015;,
  -0.199947;-0.952394;0.230144;,
  -0.128451;-0.948993;0.287946;,
  0.014561;-0.944104;0.329326;,
  0.150485;-0.945647;0.288280;,
  0.238742;-0.950029;0.201116;,
  0.302287;-0.948118;0.098463;,
  0.137108;0.950761;0.277946;,
  -0.046677;0.911030;0.409689;,
  0.211557;0.908727;0.359804;,
  0.370437;0.916973;0.148112;,
  0.406381;0.907901;-0.102814;,
  0.291480;0.951274;-0.100584;,
  -0.391933;0.910181;0.134008;,
  -0.398536;0.908199;-0.127843;,
  -0.268698;0.904781;0.330414;,
  -0.218199;0.947256;0.234722;,
  0.264159;0.916455;-0.300548;,
  0.078303;0.915877;-0.393749;,
  0.016456;0.945812;-0.324298;,
  -0.215124;0.913214;-0.346065;,
  -0.301592;0.947100;-0.109745;,
  -0.010176;0.999879;-0.011778;,
  -0.106480;-0.931732;0.347185;,
  -0.268666;-0.939150;0.214047;,
  -0.361756;-0.932005;0.022368;,
  -0.311547;-0.935202;-0.168334;,
  -0.140950;-0.942761;-0.302215;,
  0.062525;-0.934691;-0.349919;,
  0.244389;-0.937589;-0.247387;,
  0.346687;-0.935878;-0.062771;,
  0.362426;-0.924819;0.115575;,
  0.246945;-0.935769;0.251704;,
  0.084355;-0.931453;0.353950;,
  -0.198203;-0.959800;-0.198747;,
  -0.375569;-0.912543;0.161903;,
  0.193522;-0.895561;-0.400651;,
  0.292401;-0.953476;0.073384;,
  0.009522;-0.938186;0.346001;,
  0.407143;-0.317182;-0.856522;,
  -0.627974;-0.317471;-0.710535;,
  -0.918466;0.012618;-0.395299;,
  -0.935415;-0.016382;-0.353171;,
  -0.802821;-0.110921;-0.585811;,
  0.878539;0.019864;-0.477257;,
  0.656489;-0.119410;-0.744825;,
  0.878682;-0.043566;-0.475416;,
  0.389344;-0.422065;0.818702;,
  -0.349731;-0.423095;0.835870;,
  0.043488;0.851432;0.522659;,
  -0.492808;0.675573;0.548399;,
  0.911796;-0.249626;0.326060;,
  0.977967;-0.085685;0.190364;,
  0.517785;0.709867;0.477480;,
  -0.567416;-0.432909;0.700449;,
  -0.822457;-0.247682;0.512072;,
  -0.803091;0.391391;0.449286;,
  -0.885087;-0.023006;0.464857;,
  0.099776;0.729031;-0.677170;,
  0.654041;-0.460781;0.599927;,
  0.186748;0.325007;-0.927090;,
  0.909322;0.316026;0.270668;,
  -0.145027;0.683449;-0.715448;,
  0.061359;-0.541341;0.838561;,
  0.097896;0.890647;0.444032;,
  -0.055078;0.885750;0.460884;,
  0.240419;0.883196;0.402696;,
  0.348088;0.889136;0.297106;,
  0.442045;0.883944;0.152442;,
  0.468280;0.883438;-0.015847;,
  0.406625;0.885377;-0.225308;,
  -0.460644;0.887437;0.016187;,
  -0.436070;0.881467;0.181271;,
  -0.362340;0.881924;0.301530;,
  -0.196796;0.887612;0.416433;,
  0.301011;0.885978;-0.352755;,
  0.158028;0.885984;-0.435958;,
  -0.061076;0.882714;-0.465924;,
  -0.221216;0.882883;-0.414225;,
  -0.351608;0.893022;-0.280860;,
  -0.443690;0.883786;-0.148532;,
  0.337650;0.901738;-0.269928;,
  0.139613;0.903680;-0.404810;,
  0.422275;0.905375;-0.044508;,
  0.371388;0.906461;0.200994;,
  0.193806;0.904597;0.379662;,
  -0.063123;0.906336;0.417816;,
  -0.294862;0.904631;0.307732;,
  -0.415215;0.903869;0.103040;,
  -0.407535;0.903693;-0.131358;,
  -0.288923;0.901576;-0.322000;,
  -0.093451;0.902256;-0.420953;,
  0.010031;-0.999773;0.018777;,
  -0.262502;-0.425785;-0.865910;;
  219;
  3;2,16,4;,
  3;32,27,2;,
  3;2,27,16;,
  3;16,27,40;,
  3;16,40,36;,
  3;18,19,6;,
  3;15,15,15;,
  3;117,0,118;,
  3;3,2,4;,
  3;32,2,3;,
  3;7,18,6;,
  3;32,28,27;,
  3;28,40,27;,
  3;30,36,40;,
  3;33,28,32;,
  3;44,43,42;,
  3;119,120,11;,
  3;10,18,7;,
  3;5,6,19;,
  3;7,6,13;,
  3;7,13,14;,
  3;5,4,8;,
  3;5,8,9;,
  3;6,5,9;,
  3;6,9,13;,
  3;4,16,8;,
  3;14,17,35;,
  3;14,35,34;,
  3;1,10,11;,
  3;7,11,10;,
  3;118,14,34;,
  3;118,34,121;,
  3;17,12,35;,
  3;11,7,14;,
  3;11,14,118;,
  3;13,9,12;,
  3;13,12,17;,
  3;8,16,36;,
  3;9,8,36;,
  3;9,36,12;,
  3;14,13,17;,
  3;3,4,5;,
  3;1,11,120;,
  3;22,122,26;,
  3;20,22,21;,
  3;24,44,42;,
  3;37,43,44;,
  3;37,44,38;,
  3;29,31,30;,
  3;29,30,28;,
  3;43,37,31;,
  3;43,31,29;,
  3;28,30,40;,
  3;38,123,35;,
  3;38,35,41;,
  3;21,26,24;,
  3;24,26,44;,
  3;39,12,36;,
  3;39,36,30;,
  3;122,124,123;,
  3;117,123,38;,
  3;41,35,12;,
  3;41,12,39;,
  3;26,117,38;,
  3;26,38,44;,
  3;37,41,39;,
  3;37,39,31;,
  3;31,39,30;,
  3;38,41,37;,
  3;29,28,33;,
  3;26,21,22;,
  4;23,125,24,25;,
  4;23,25,10,126;,
  4;45,45,45,45;,
  3;61,127,128;,
  4;67,65,65,67;,
  3;129,130,66;,
  4;64,64,64,64;,
  3;61,131,127;,
  3;51,46,50;,
  4;49,52,52,49;,
  3;132,47,133;,
  3;61,128,51;,
  3;61,51,62;,
  4;53,48,48,53;,
  3;51,134,135;,
  3;51,135,46;,
  4;136,136,136,136;,
  3;128,134,51;,
  3;50,46,47;,
  3;47,46,135;,
  4;52,53,53,52;,
  3;47,135,133;,
  4;65,54,54,65;,
  3;137,129,59;,
  4;60,138,138,60;,
  3;130,139,66;,
  3;66,139,57;,
  3;66,57,58;,
  4;140,60,60,140;,
  3;139,131,57;,
  3;59,129,66;,
  3;59,66,58;,
  4;54,55,55,54;,
  4;137,59,63,141;,
  4;55,49,49,55;,
  4;63,47,132,141;,
  4;50,47,63,56;,
  4;58,56,63,59;,
  4;51,50,56,62;,
  4;57,62,56,58;,
  4;57,131,61,62;,
  3;108,69,68;,
  3;69,108,107;,
  3;107,70,69;,
  3;107,71,70;,
  3;71,107,106;,
  3;106,72,71;,
  3;72,106,105;,
  3;105,73,72;,
  3;105,74,73;,
  3;105,75,74;,
  3;75,105,104;,
  3;104,76,75;,
  3;76,104,103;,
  3;103,77,76;,
  3;77,103,102;,
  3;102,78,77;,
  3;102,79,78;,
  3;102,80,79;,
  3;80,102,101;,
  3;101,81,80;,
  3;81,101,111;,
  3;111,82,81;,
  3;82,111,110;,
  3;110,83,82;,
  3;110,84,83;,
  3;84,110,109;,
  3;84,109,108;,
  3;108,68,84;,
  3;100,94,85;,
  3;86,85,94;,
  3;142,87,86;,
  3;85,86,87;,
  3;86,143,142;,
  3;87,142,144;,
  3;145,88,87;,
  3;85,87,88;,
  3;87,144,145;,
  3;88,90,85;,
  3;100,85,90;,
  3;88,145,146;,
  3;147,89,88;,
  3;90,88,89;,
  3;88,146,147;,
  3;148,95,89;,
  3;90,89,95;,
  3;89,147,148;,
  3;100,99,94;,
  3;91,149,150;,
  3;149,91,92;,
  3;99,92,91;,
  3;151,93,91;,
  3;91,150,151;,
  3;94,91,93;,
  3;91,94,99;,
  3;86,152,143;,
  3;152,86,93;,
  3;93,151,152;,
  3;94,93,86;,
  3;95,148,153;,
  3;154,96,95;,
  3;90,95,96;,
  3;96,97,90;,
  3;100,90,97;,
  3;95,153,154;,
  3;96,154,155;,
  3;155,98,96;,
  3;97,96,98;,
  3;98,99,97;,
  3;100,97,99;,
  3;98,155,156;,
  3;157,92,98;,
  3;99,98,92;,
  3;98,156,157;,
  3;92,157,158;,
  3;92,158,149;,
  3;100,159,160;,
  3;100,161,159;,
  3;100,162,161;,
  3;100,163,162;,
  3;100,164,163;,
  3;100,165,164;,
  3;100,166,165;,
  3;100,167,166;,
  3;100,168,167;,
  3;100,169,168;,
  3;100,160,169;,
  3;170,101,102;,
  3;170,102,103;,
  3;170,103,104;,
  3;170,104,105;,
  3;170,105,106;,
  3;170,106,107;,
  3;170,107,108;,
  3;170,108,109;,
  3;170,109,110;,
  3;170,110,111;,
  3;170,111,101;,
  3;20,117,22;,
  3;0,117,171;,
  4;114,112,0,171;,
  4;115,114,20,21;,
  3;112,114,115;,
  4;113,116,23,126;,
  4;112,113,1,0;,
  3;116,113,112;,
  4;116,115,125,23;,
  3;116,112,115;;
 }
 MeshTextureCoords {
  252;
  0.857280;0.036430;,
  0.842510;0.017840;,
  0.835800;0.045270;,
  0.874150;0.039110;,
  0.874150;0.011570;,
  0.892890;0.201830;,
  0.878600;0.181500;,
  0.878730;0.202420;,
  0.888680;0.220890;,
  0.819150;0.115110;,
  0.820040;0.089540;,
  0.792680;0.100900;,
  0.884770;0.347690;,
  0.892720;0.328250;,
  0.884440;0.326670;,
  0.133500;0.357680;,
  0.139440;0.367550;,
  0.142410;0.354740;,
  0.849080;0.055290;,
  0.859460;0.058490;,
  0.794060;0.116720;,
  0.891020;0.036430;,
  0.905790;0.017840;,
  0.845230;0.187970;,
  0.853400;0.198620;,
  0.855510;0.220200;,
  0.868980;0.221000;,
  0.864560;0.201990;,
  0.899220;0.055290;,
  0.912500;0.045270;,
  0.954240;0.116720;,
  0.955610;0.100900;,
  0.929150;0.115110;,
  0.156400;0.361020;,
  0.156770;0.343930;,
  0.840300;0.142060;,
  0.818770;0.143820;,
  0.824690;0.065670;,
  0.794000;0.085250;,
  0.964310;0.296630;,
  0.966310;0.279620;,
  0.932750;0.272180;,
  0.930610;0.288090;,
  0.957550;0.228560;,
  0.937790;0.202560;,
  0.919060;0.217120;,
  0.928520;0.236830;,
  0.966500;0.262650;,
  0.933460;0.258910;,
  0.904010;0.198330;,
  0.911900;0.285930;,
  0.910970;0.269620;,
  0.893940;0.268620;,
  0.898030;0.285640;,
  0.322970;0.344620;,
  0.339750;0.337430;,
  0.329020;0.333020;,
  0.818150;0.163430;,
  0.835720;0.158750;,
  0.904200;0.311870;,
  0.890260;0.309510;,
  0.909980;0.257510;,
  0.890890;0.238680;,
  0.892310;0.256210;,
  0.938040;0.348550;,
  0.954020;0.325610;,
  0.919950;0.318590;,
  0.910000;0.336740;,
  0.906920;0.238300;,
  0.902130;0.219940;,
  0.174130;0.347280;,
  0.100650;0.361020;,
  0.114640;0.354740;,
  0.100280;0.343930;,
  0.080150;0.367670;,
  0.082910;0.347280;,
  0.908000;0.142060;,
  0.929530;0.143820;,
  0.825440;0.272770;,
  0.792020;0.280620;,
  0.794210;0.297600;,
  0.827750;0.288650;,
  0.800200;0.229460;,
  0.829300;0.237380;,
  0.838560;0.217560;,
  0.819660;0.203230;,
  0.791630;0.263650;,
  0.824580;0.259510;,
  0.846400;0.286280;,
  0.860240;0.285820;,
  0.864200;0.268760;,
  0.847180;0.269950;,
  0.401870;0.344620;,
  0.395830;0.333020;,
  0.385100;0.337430;,
  0.912580;0.158750;,
  0.930150;0.163430;,
  0.850930;0.238590;,
  0.866960;0.238810;,
  0.866080;0.328410;,
  0.874340;0.326740;,
  0.868280;0.309660;,
  0.854380;0.312140;,
  0.848050;0.257830;,
  0.865720;0.256340;,
  0.821080;0.349220;,
  0.848860;0.337090;,
  0.838740;0.319030;,
  0.804830;0.326470;,
  0.923610;0.065670;,
  0.373520;0.356860;,
  0.390230;0.351620;,
  0.372090;0.339640;,
  0.351330;0.356860;,
  0.352760;0.339640;,
  0.334620;0.351620;,
  0.859460;0.058490;,
  0.874150;0.039110;,
  0.874150;0.059490;,
  0.955610;0.100900;,
  0.929150;0.115110;,
  0.928250;0.089540;,
  0.888830;0.058490;,
  0.888830;0.058490;,
  0.860240;0.076210;,
  0.844110;0.092400;,
  0.860230;0.093880;,
  0.840300;0.142060;,
  0.819150;0.115110;,
  0.842740;0.113420;,
  0.874150;0.077300;,
  0.820040;0.089540;,
  0.824690;0.065670;,
  0.845120;0.073300;,
  0.849080;0.055290;,
  0.908000;0.142060;,
  0.905560;0.113420;,
  0.923610;0.065670;,
  0.954300;0.085250;,
  0.954300;0.085250;,
  0.903170;0.073300;,
  0.904180;0.092400;,
  0.899220;0.055290;,
  0.886300;0.140720;,
  0.886300;0.140720;,
  0.887510;0.112390;,
  0.862000;0.140720;,
  0.862000;0.140720;,
  0.874150;0.111940;,
  0.860790;0.112390;,
  0.874150;0.094410;,
  0.888070;0.093880;,
  0.888060;0.076210;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.045450;0.000000;,
  0.090910;0.000000;,
  0.000000;0.000000;,
  0.136360;0.000000;,
  0.181820;0.000000;,
  0.227270;0.000000;,
  0.272730;0.000000;,
  0.318180;0.000000;,
  0.363640;0.000000;,
  0.409090;0.000000;,
  0.454550;0.000000;,
  0.500000;0.000000;,
  0.545450;0.000000;,
  0.590910;0.000000;,
  0.636360;0.000000;,
  0.681820;0.000000;,
  0.727270;0.000000;,
  0.772730;0.000000;,
  0.818180;0.000000;,
  0.863640;0.000000;,
  0.909090;0.000000;,
  0.954550;0.000000;,
  1.000000;0.000000;,
  0.045450;1.000000;,
  0.090910;1.000000;,
  0.136360;1.000000;,
  0.181820;1.000000;,
  0.227270;1.000000;,
  0.272730;1.000000;,
  0.318180;1.000000;,
  0.363640;1.000000;,
  0.409090;1.000000;,
  0.454550;1.000000;,
  0.500000;1.000000;,
  0.545450;1.000000;,
  0.590910;1.000000;,
  0.636360;1.000000;,
  0.681820;1.000000;,
  0.727270;1.000000;,
  0.772730;1.000000;,
  0.818180;1.000000;,
  0.863640;1.000000;,
  0.909090;1.000000;,
  0.954550;1.000000;,
  1.000000;1.000000;,
  0.093540;0.373490;,
  0.111330;0.365390;,
  0.129910;0.375820;,
  0.132050;0.369130;,
  0.106930;0.379310;,
  0.080150;0.367670;,
  0.139440;0.367550;,
  0.082910;0.347280;,
  0.334620;0.351620;,
  0.351330;0.356860;;
 }
}