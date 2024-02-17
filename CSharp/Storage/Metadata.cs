namespace Firebase;

using Godot.Collections;


public struct Metadata
{
    public string Bucket { get; set; }
    public string CacheControl { get; set; }
    public string ContentDisposition { get; set; }
    public string ContentEncoding { get; set; }
    public string ContentLanguage { get; set; }
    public string ContentType { get; set; }
    public long CreationTime { get; set; }
    public Dictionary CustomMetadata { get; set; }
    public long Generation { get; set; }
    public long MetadataGeneration { get; set; }
    public string Name { get; set; }
    public string Path { get; set; }
    public long SizeBytes { get; set; }
    public long UpdatedTime { get; set; }

    public Metadata(Dictionary data)
    {
        Bucket = data.ContainsKey("bucket") ? data["bucket"].AsString() : null;
        CacheControl = data.ContainsKey("cache_control") ? data["cache_control"].AsString() : null;
        ContentDisposition = data.ContainsKey("content_disposition") ? data["content_disposition"].AsString() : null;
        ContentEncoding = data.ContainsKey("content_encoding") ? data["content_encoding"].AsString() : null;
        ContentLanguage = data.ContainsKey("content_language") ? data["content_language"].AsString() : null;
        ContentType = data.ContainsKey("content_type") ? data["content_type"].AsString() : null;
        CreationTime = data.ContainsKey("creation_time") ? data["creation_time"].AsInt64() : 0L;
        CustomMetadata = data.ContainsKey("custom_metadata") ? data["custom_metadata"].AsGodotDictionary() : null;
        Generation = data.ContainsKey("generation") ? data["generation"].AsInt64() : 0L;
        MetadataGeneration = data.ContainsKey("metadata_generation") ? data["metadata_generation"].AsInt64() : 0L;
        Name = data.ContainsKey("name") ? data["name"].AsString() : null;
        Path = data.ContainsKey("path") ? data["path"].AsString() : null;
        SizeBytes = data.ContainsKey("size_bytes") ? data["size_bytes"].AsInt64() : 0L;
        UpdatedTime = data.ContainsKey("updated_time") ? data["updated_time"].AsInt64() : 0L;
    }

    public readonly Dictionary AsDictionary()
    {
        Dictionary result = new();

        if (string.IsNullOrEmpty(Bucket)) result["bucket"] = Bucket;
        if (string.IsNullOrEmpty(CacheControl)) result["cache_control"] = CacheControl;
        if (string.IsNullOrEmpty(ContentDisposition)) result["content_disposition"] = ContentDisposition;
        if (string.IsNullOrEmpty(ContentEncoding)) result["content_encoding"] = ContentEncoding;
        if (string.IsNullOrEmpty(ContentLanguage)) result["content_language"] = ContentLanguage;
        if (string.IsNullOrEmpty(ContentType)) result["content_type"] = ContentType;
        if (CreationTime != 0L) result["creation_time"] = CreationTime;
        if (CustomMetadata != null) result["custom_metadata"] = CustomMetadata;
        if (Generation != 0L) result["generation"] = Generation;
        if (MetadataGeneration != 0L) result["metadata_generation"] = MetadataGeneration;
        if (string.IsNullOrEmpty(Name)) result["name"] = Name;
        if (string.IsNullOrEmpty(Path)) result["path"] = Path;
        if (SizeBytes != 0L) result["size_bytes"] = SizeBytes;
        if (UpdatedTime != 0L) result["updated_time"] = UpdatedTime;

        return result;
    }
}