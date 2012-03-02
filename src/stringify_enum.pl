#!/usr/bin/perl

while (<>) {
	if (/enum\s+(\w+)?\{(.+?)\}(\s+(\w+)?)/) {
		@idents = split /\s*,\s*/, $2;
		$entity = $1 || $4;
		print "switch ($entity) {\n";
		foreach $ident (@idents) {
			print "    case $ident: return CGString__new(\"$ident\"); break;\n";
		}
		print "    default: CGAppState_THROW(CGAppState__getInstance(), Severity_warning, BNFExceptionID_Unknown$entity, \"No such $entity\"); return CGString__new(\"unkown $entity\"); break;\n";
		print "}\n";
	}
}
