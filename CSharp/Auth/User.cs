namespace Firebase;

using Godot.Collections;
using System.Collections.Generic;
using System.Linq;

public struct ProviderData
{
    public string Uid { get; set; }
    public string DisplayName { get; set; }
    public string Email { get; set; }
    public string PhoneNumber { get; set; }
    public string PhotoUrl { get; set; }
    public LoginProvider ProviderType { get; set; }
}
public class User
{
    public string Uid { get; private set; }
    public string Email { get; private set; }
    public bool IsEmailVerified { get; private set; }
    public string DisplayName { get; private set; }
    public string PhoneNumber { get; private set; }
    public string PhotoUrl { get; private set; }
    public List<ProviderData> Providers { get; private set; }
    public LoginProvider ProviderType => IsAnonymous ? LoginProvider.Anonymous : Providers[0].ProviderType;
    public bool IsAnonymous { get; private set; }

    public long LastLoginAt { get; private set; }
    public long CreatedAt { get; private set; }

    public User(Dictionary userData)
    {
        Uid = userData["uid"].AsString();
        Email = userData["email"].AsString();
        IsEmailVerified = userData["is_email_verified"].AsBool();
        DisplayName = userData["display_name"].AsString();
        PhotoUrl = userData["photo_url"].AsString();
        PhoneNumber = userData["phone_number"].AsString();
        IsAnonymous = userData["is_anonymous"].AsBool();
        Providers = new();
        foreach (Dictionary provider in userData["provider_data"].AsGodotArray().Select(v => (Dictionary)v))
        {
            Providers.Add(new()
            {
                Uid = provider["uid"].AsString(),
                DisplayName = provider["display_name"].AsString(),
                Email = provider["display_name"].AsString(),
                PhoneNumber = provider["phone_number"].AsString(),
                PhotoUrl = provider["photo_url"].AsString(),
                ProviderType = Auth.ProviderFromString(provider["provider_id"].AsString())
            });
        }
        LastLoginAt = userData["last_sign_in_timestamp"].AsInt64();
        CreatedAt = userData["creation_timestamp"].AsInt64();
    }
}
