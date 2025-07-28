param (
)

$Global:ErrorActionPreference = "Stop"

$ProjectDirectory = (Resolve-Path -Path "${PSScriptRoot}/../..").Path.Replace('\', '/')
$ProjectLocalDirectory = "${ProjectDirectory}/.local"
$ProjectCertificateDirectory = "${ProjectLocalDirectory}/certificate"
$ProjectTemporaryDirectory = "${ProjectLocalDirectory}/temporary"
$ProjectDistributionDirectory = "${ProjectLocalDirectory}/distribution"

function My-PackZip(
	[Parameter(Mandatory)]
	[String] $Name,
	[Parameter(Mandatory)]
	[String] $Source,
	[Parameter(Mandatory)]
	[String] $Destination
) {
	New-Item -Force -ItemType "Directory" -Path "${ProjectTemporaryDirectory}/PackZip"
	New-Item -Force -ItemType "Directory" -Path "${ProjectTemporaryDirectory}/PackZip/${Name}"
	Copy-Item -Force -Recurse -Path "${Source}\*" -Destination "${ProjectTemporaryDirectory}/PackZip/${Name}"
	Compress-Archive -Force -CompressionLevel "Optimal" -Path "${ProjectTemporaryDirectory}/PackZip/${Name}" -DestinationPath "${ProjectTemporaryDirectory}/PackZip/${Name}.zip"
	Copy-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/PackZip/${Name}.zip" -Destination "${Destination}"
	Remove-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/PackZip"
	return
}

function My-PackMsix(
	[Parameter(Mandatory)]
	[String] $Name,
	[Parameter(Mandatory)]
	[String] $Source,
	[Parameter(Mandatory)]
	[String] $Destination
) {
	New-Item -Force -ItemType "Directory" -Path "${ProjectTemporaryDirectory}/PackMsix"
	New-Item -Force -ItemType "Directory" -Path "${ProjectTemporaryDirectory}/PackMsix/${Name}"
	Copy-Item -Force -Recurse -Path "${Source}\*" -Destination "${ProjectTemporaryDirectory}/PackMsix/${Name}"
	makepri "createconfig" "/cf" "${ProjectTemporaryDirectory}/PackMsix/${Name}/priconfig.xml" "/dq" "en-US" "/o"
	makepri "new" "/cf" "${ProjectTemporaryDirectory}/PackMsix/${Name}/priconfig.xml" "/pr" "${ProjectTemporaryDirectory}/PackMsix/${Name}" "/of" "${ProjectTemporaryDirectory}/PackMsix/${Name}/resources.pri" "/o"
	makeappx "pack" "/o" "/d" "${ProjectTemporaryDirectory}/PackMsix/${Name}" "/p" "${ProjectTemporaryDirectory}/PackMsix/${Name}.msix"
	Copy-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/PackMsix/${Name}.msix" -Destination "${Destination}"
	Remove-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/PackMsix"
	return
}

function My-PackDmg(
	[Parameter(Mandatory)]
	[String] $Name,
	[Parameter(Mandatory)]
	[String] $Source,
	[Parameter(Mandatory)]
	[String] $Destination
) {
	New-Item -Force -ItemType "Directory" -Path "${ProjectTemporaryDirectory}/PackDmg"
	Copy-Item -Force -Recurse -Path "${Source}" -Destination "${ProjectTemporaryDirectory}/PackDmg/${Name}.app"
	create-dmg "${ProjectTemporaryDirectory}/PackDmg/${Name}.dmg" "${ProjectTemporaryDirectory}/PackDmg/${Name}.app"
	Copy-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/PackDmg/${Name}.dmg" -Destination "${Destination}"
	Remove-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/PackDmg"
	return
}

function My-PackIpa(
	[Parameter(Mandatory)]
	[String] $Name,
	[Parameter(Mandatory)]
	[String] $Source,
	[Parameter(Mandatory)]
	[String] $Destination
) {
	New-Item -Force -ItemType "Directory" -Path "${ProjectTemporaryDirectory}/PackIpa"
	Copy-Item -Force -Recurse -Path "${Source}" -Destination "${ProjectTemporaryDirectory}/PackIpa/Payload/${Name}.app"
	Compress-Archive -Force -CompressionLevel "Optimal" -Path "${ProjectTemporaryDirectory}/PackIpa/Payload" -DestinationPath "${ProjectTemporaryDirectory}/PackIpa/Payload.ipa"
	Copy-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/PackIpa/Payload.ipa" -Destination "${Destination}"
	Remove-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/PackIpa"
	return
}

function My-SignMsix(
	[Parameter(Mandatory)]
	[String] $Source,
	[Parameter(Mandatory)]
	[String] $Destination
) {
	$CertificateFile = "${ProjectCertificateDirectory}/file.pfx"
	$CertificatePassword = Get-Content -Path "${ProjectCertificateDirectory}/password.txt"
	New-Item -Force -ItemType "Directory" -Path "${ProjectTemporaryDirectory}/SignMsix"
	Copy-Item -Force -Recurse -Path "${Source}" -Destination "${ProjectTemporaryDirectory}/SignMsix/unsigned.msix"
	signtool "sign" "/q" "/fd" "SHA256" "/f" "${CertificateFile}" "/p" "${CertificatePassword}" "${ProjectTemporaryDirectory}/SignMsix/unsigned.msix"
	Copy-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/SignMsix/unsigned.msix" -Destination "${Destination}"
	Remove-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/SignMsix"
	return
}

function My-SignApk(
	[Parameter(Mandatory)]
	[String] $Source,
	[Parameter(Mandatory)]
	[String] $Destination
) {
	$CertificateFile = "${ProjectCertificateDirectory}/file.jks"
	$CertificatePassword = Get-Content -Path "${ProjectCertificateDirectory}/password.txt"
	New-Item -Force -ItemType "Directory" -Path "${ProjectTemporaryDirectory}/SignApk"
	Copy-Item -Force -Recurse -Path "${Source}" -Destination "${ProjectTemporaryDirectory}/SignApk/unsigned.apk"
	zipalign "-f" "-p" "4" "${ProjectTemporaryDirectory}/SignApk/unsigned.apk" "${ProjectTemporaryDirectory}/SignApk/aligned.apk"
	apksigner "sign" "--ks" "${CertificateFile}" "--ks-pass" "pass:${CertificatePassword}" "${ProjectTemporaryDirectory}/SignApk/aligned.apk"
	Copy-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/SignApk/aligned.apk" -Destination "${Destination}"
	Remove-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/SignApk"
	return
}
