#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "test"                         //change this
 
const char WIFI_SSID[] = "Embedded";               //change this
const char WIFI_PASSWORD[] = "1234567890@";           //change this
const char AWS_IOT_ENDPOINT[] = "a3iuhtzz3ex62l-ats.iot.ap-south-1.amazonaws.com";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAJNqfZnX/r8yBvsUo77oOlJqlXyHMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMjAzMDkwNDUx
MTNaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDOxSMDe5j3DY/HEGMT
DiKoxOxAW0V6JfT7vpREGeEPzU1aHbbTLnsbpx8DCp+5lRJRCk93ajX3TgCw8DG2
MQGp1quEx5UKeM579CIyIsJFUxQx/KdLaYTxwQvx/QtBlHTLMm9wz5hVmt5j40VE
rk4XEuecDx8wsRIUSsxYdYWjZLrOI5TUumYcsE49227Oq9ibTsR/nfCwI3HTy1GL
BbkE/Niwkvykje2kxFFd4GPC3h+ER3xh5y77X4LJabQ0/U/Hp+KuF21Ak/MJfl33
ysVWD69HsnhnPY0A0RRXsTChan4JCsoHc248M+aQ4DM0EY3cEsG81YUptUlvZ0R8
5Z8/AgMBAAGjYDBeMB8GA1UdIwQYMBaAFEn4Qcu89mkzLzeQgUBHWoiiVeRiMB0G
A1UdDgQWBBTE/JcdanynxOQP3AuwzpZq0JJ4djAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAcC+iCRc+Gpi7YmfH2l+ixeQ3
uPFzJ7v3+LQ4qj1tnGQQCq3HinvkwVxo7unyi3UANUsW1IyI1XMKiFLE+rhyKNGB
8+RqE26tEEnp4h2XJYba8UZYXg198Jlv1AonRlqtAfHHD+nlyPUVILns8RCkbGti
kMeY+/7wrKQ3G1OjbbhAA5wiqpQdgoDQIWN6WWxhE19rEh/onVGXvZefDr/ZIKJj
yFoK74V3qEjEs0y1sDB9Wjd08EBx2RYubW4Nf0NSLXyV2dvu/cwz2Q7ArlOX5Crv
OjcabR+cQDlK5ZFw1GA2tZd61Du2Q0oWHt7539Be/BsTVPJwx2SnaMj/w8NAeA==
-----END CERTIFICATE-----

 
 
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAzsUjA3uY9w2PxxBjEw4iqMTsQFtFeiX0+76URBnhD81NWh22
0y57G6cfAwqfuZUSUQpPd2o1904AsPAxtjEBqdarhMeVCnjOe/QiMiLCRVMUMfyn
S2mE8cEL8f0LQZR0yzJvcM+YVZreY+NFRK5OFxLnnA8fMLESFErMWHWFo2S6ziOU
1LpmHLBOPdtuzqvYm07Ef53wsCNx08tRiwW5BPzYsJL8pI3tpMRRXeBjwt4fhEd8
Yecu+1+CyWm0NP1Px6firhdtQJPzCX5d98rFVg+vR7J4Zz2NANEUV7EwoWp+CQrK
B3NuPDPmkOAzNBGN3BLBvNWFKbVJb2dEfOWfPwIDAQABAoIBADpr3uG8CVM8hLQ1
ABlBNC/FPA9O+olxsg9zIIdBXKLPWhytKpvulSASDyQX21tfMEaviBZwbwEKDA3y
P8l/fT3Uhq/EiGrmCUliiErsIUHfIGSjQ01+ae64obxkx8pmvL0w6eXQ8fS2BkHo
1Ah8y1mfoMsqRqDGVxpeq/AOfIzWhe/qaD0xzV7/ZeTxZc1YfKWG25bi1wr4o9A2
7EolQOOOCmbzEtuyplbSEgJCztUgmj+4GiuiTSXP1AEq6JnSOb9lEfIMQ14oi2sp
eLHMLW0RdtwCen5qM2hgiuZHVvT5Uzg3zXLj4Gg0eVxCHz4PTAZBxuxM1TidCDAA
tGRJE/ECgYEA8xZ7X4nJRLYYnIisgKiBlai+K3sFqgNV318HHTzNnTayNoHhFWGq
kIXomm86cxIt7u1a8NAHuoETooRBUW32Sfl9hPRTk5mt46vFqdJ/Kj2RLcT23JRH
9G67dsj1Kf5wrZa5lHV5A2D4+bV4ifA7nBaAytDVf2T6lM+zQwhwuQcCgYEA2cDN
9mTV+FfGc8e/FqV9lfCKFaCejb0ih9ASPX+RQKqRgHL/k1wVlwcLtDfJIz/lk8fK
ftOh7B5twAs3wzmugZ1Z+aTUtgvg786wLPIr+IlPC5NjMKOnatRAkPqp+xolA95Y
tQO6Fb9qaCAqGtJ2UJc3nWOla1JdQ6T1S9ymcgkCgYEAtqDcXcGx8apxQSE5oIt4
l8yn2xjY/4rz+BZ3zNKCk6ulJmhbh2nR8DR6tZP9MLimw8uVODjCpa8V5lJAlIHt
uFP1pm69X7Xku5iwr06S9VQ9/0nsQXoxuQV8V9jQwBUF6JTFpjC/kvdG9qwM/Gz9
C96oMjfFPqnJzQViY2ky7v8CgYBI7liNU8yOdueS063eqeLcG0pEYB16n4Q3NKtH
SbiejgJZoSqUcl0Qzl1PKPgHBFmiG2JpA/zRlodvsNImYuVdqBQAlwJfTq8x4moD
Q8ESZvrAh8bK/aUPFLPjWvEfluwa1ZekWPE0Y34CDfPMM/1IlL0dy8gVn+p4w4Ec
vXRJ8QKBgC2ExgCmt0xxj32YWIxr58hmp4ABcUwSdRItqYs+v0HhEDSSbG6ZKaTF
uZfPQBc+rnLNL4HLpMuQX934Cx5dRrtWeqoU4MDyo0/CiBCdiQr/DlKkTA6tMlt8
9FewrFtEm1wvy1B6rWZWJE/NYhCAZfX2p9YZIVtgalq7VofWg1dw
-----END RSA PRIVATE KEY-----

 
 
)KEY";
