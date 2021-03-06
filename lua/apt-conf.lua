-- vi: sw=4 ts=4 noexpandtab

apt_conf_template = [[
Dir 
{
   ArchiveDir "POOLTOP";
   FileListDir "POOLTOP/indices";
};

Default
{
   Packages::Compress ". gzip bzip2";
   Sources::Compress ". gzip bzip2";
   Contents::Compress "gzip";
   DeLinkLimit 0;
   MaxContentsChange 25000;
   FileMode 0664;
}

]]

function generate_apt_conf ()
    -- Settings is a table:
    -- SUITES: A table with a list of suites that we generate Indexes for.
    -- SUITE_SETTINGS: A table with suites as keys. Each value should in turn be
    -- a table with following keys:
     -- arches        -- list of architectures for suite.
     -- components    -- list of components for suite
     -- bin_list_path -- path to the binary file list
     -- src_list_path -- path to the source file list
     -- bin_override  -- path to the binary overrides
     -- src_override  -- path to the source overrides
     apt_conf = string.gsub(apt_conf_template, "POOLTOP", Config["repo_dir"])

     for suite, tab in pairs(Suites) do
	 -- Main logic.
	 -- Start a tree for this suite.
	 apt_conf = apt_conf .. "tree \"dists/" .. suite .. "\"\n{\n"
	 suite_conf = settings["SUITE_SETTINGS"][suite]
	 apt_conf = apt_conf .. "  BinCacheDB \"/tmp/" .. suite .. ".db\";\n"
	 apt_conf = apt_conf .. "  FileList \"" .. suite_conf["bin_list_path"] .. "\";\n"
	 apt_conf = apt_conf .. "  SourceFileList \"" .. suite_conf["src_list_path"] .. "\";\n"
	 apt_conf = apt_conf .. "  Sections \"" .. table.concat(tab["components"], " ") .. "\";\n"
	 apt_conf = apt_conf .. "  Architectures \"" .. table.concat(tab["arches"], " ") .. " source\";\n"
	 if suite_conf["bin_override"] then
	     apt_conf = apt_conf .. "  BinOverride \"" .. suite_conf["bin_override"] .. "\";\n"
	 end
	 if suite_conf["src_override"] then
	     apt_conf = apt_conf .. "  SrcOverride \"" .. suite_conf["src_override"] .. "\";\n"
	 end
	 apt_conf = apt_conf .."}\n"
     end

     return apt_conf
end

-- settings = {}
-- 
-- settings["POOLTOP"] = "/root/"
-- settings["SUITES"] = { "sarge" }
-- settings["SUITE_SETTINGS"] = {}
-- settings["SUITE_SETTINGS"]["sarge"] = {}
-- settings["SUITE_SETTINGS"]["sarge"]["bin_list_path"] = "/root/foo/bar_bin/"
-- settings["SUITE_SETTINGS"]["sarge"]["src_list_path"] = "/root/foo/baz_src/"
-- settings["SUITE_SETTINGS"]["sarge"]["components"] = "host-tools main security"
-- settings["SUITE_SETTINGS"]["sarge"]["arches"] = "i386 powerpc mips"
-- 
-- generate_apt_conf(settings)
