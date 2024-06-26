param (
)

$Global:ErrorActionPreference = "Stop"

$ProjectDirectory = (Resolve-Path -Path "${PSScriptRoot}/../..").Path.Replace('\', '/')
$ProjectDistributionDirectory = "${ProjectDirectory}/.distribution"
$ProjectTemporaryDirectory = "${ProjectDirectory}/.temporary"

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

function My-PackDmg(
	[Parameter(Mandatory)]
	[String] $Name,
	[Parameter(Mandatory)]
	[String] $Source,
	[Parameter(Mandatory)]
	[String] $Destination
) {
	New-Item -Force -ItemType "Directory" -Path "${ProjectTemporaryDirectory}/PackDmg"
	Move-Item -Force -Path "${Source}" -Destination "${ProjectTemporaryDirectory}/PackDmg/${Name}.app"
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
	$CertificateFile = "${ProjectDirectory}/common/certificate/file.pfx"
	$CertificatePassword = Get-Content -Path "${ProjectDirectory}/common/certificate/password.txt"
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
	$CertificateFile = "${ProjectDirectory}/common/certificate/file.jks"
	$CertificatePassword = Get-Content -Path "${ProjectDirectory}/common/certificate/password.txt"
	New-Item -Force -ItemType "Directory" -Path "${ProjectTemporaryDirectory}/SignApk"
	Copy-Item -Force -Recurse -Path "${Source}" -Destination "${ProjectTemporaryDirectory}/SignApk/unsigned.apk"
	zipalign "-f" "-p" "4" "${ProjectTemporaryDirectory}/SignApk/unsigned.apk" "${ProjectTemporaryDirectory}/SignApk/aligned.apk"
	apksigner "sign" "--ks" "${CertificateFile}" "--ks-pass" "pass:${CertificatePassword}" "${ProjectTemporaryDirectory}/SignApk/aligned.apk"
	Copy-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/SignApk/aligned.apk" -Destination "${Destination}"
	Remove-Item -Force -Recurse -Path "${ProjectTemporaryDirectory}/SignApk"
	return
}
