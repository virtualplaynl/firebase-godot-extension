#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/object.hpp>
#include <firebase/app.h>
#include <firebase/auth.h>
#include "GodotFirebaseObjects.h"

using namespace firebase;
using namespace firebase::auth;

namespace godot
{
    GFB_CALLBACK0_H(DeleteUserTask);
    GFB_CALLBACK1_H(SignInTask, Dictionary, user_data);
    GFB_CALLBACK1_H(CreateUserTask, Dictionary, user_data);
    GFB_CALLBACK1_H(GetAuthTokenTask, String, token);
    GFB_CALLBACK1_H(UserLinkTask, Dictionary, user_data);
    GFB_CALLBACK1_H(UserReauthenticateTask, Dictionary, user_data);
    GFB_CALLBACK0_H(UserReloadTask);
    GFB_CALLBACK0_H(UserSendEmailVerificationTask);
    GFB_CALLBACK1_H(UserUnlinkTask, Dictionary, user_data);
    GFB_CALLBACK1_H(UserUpdateEmailTask, String, email);
    GFB_CALLBACK1_H(UserUpdatePasswordTask, String, password);
    GFB_CALLBACK1_H(UserUpdatePhoneNumberTask, Dictionary, user_data);
    GFB_CALLBACK2_H(UserUpdateProfileTask, String, display_name, String, photo_url);
    GFB_CALLBACK0_H(SendPasswordResetTask);

    class GodotFirebaseAuth : public Object,
                              AuthStateListener,
                              IdTokenListener,
                              PhoneAuthProvider::Listener
    {
        GDCLASS(GodotFirebaseAuth, Object);

    protected:
        static void _bind_methods();
        Auth *auth;
        std::map<String, FederatedOAuthProvider> providers;
        std::map<String, Credential> credentials;

        std::string phoneVerificationCode;
        PhoneAuthProvider::ForceResendingToken phoneResendingToken;
        PhoneAuthCredential phoneCredential;

        FederatedOAuthProvider registerProvider(String provider, Array scopes, Dictionary customParameters);

    public:
        GodotFirebaseAuth();
        ~GodotFirebaseAuth();

        void init(App *app);

        SignInTask *signInAnonymously();
        Credential getEmailCredential(String email, String password);
        SignInTask *signInWithEmailAndPassword(String email, String password);
        SignInTask *signInWithCredentialFrom(String providerId);
        SignInTask *signInWithCredential(Credential credential);
        SignInTask *signInWithCustomToken(String token);
        SignInTask *signInWithProviderId(String providerId);
        SignInTask *signInWithProvider(FederatedAuthProvider *provider, String providerId);
        void setLanguageCode(String languageCode);
        CreateUserTask *createUserWithEmailAndPassword(String email, String password);
        Dictionary getUser();
        DeleteUserTask *userDelete();
        GetAuthTokenTask *userGetToken(bool forceRefresh);
        UserLinkTask *userLinkWithCredentialFrom(String providerId);
        UserLinkTask *userLinkWithCredential(Credential credential);
        UserLinkTask *userLinkWithProviderId(String providerId);
        UserLinkTask *userLinkWithProvider(FederatedAuthProvider *provider, String providerId);
        UserReauthenticateTask *userReauthenticateWithCredentialFrom(String providerId);
        UserReauthenticateTask *userReauthenticateWithCredential(Credential credential);
        UserReauthenticateTask *userReauthenticateWithProviderId(String providerId);
        UserReauthenticateTask *userReauthenticateWithProvider(FederatedAuthProvider *provider, String providerId);
        UserReloadTask *userReload();
        UserSendEmailVerificationTask *userSendEmailVerification();
        UserUnlinkTask *userUnlink(String providerId);
        UserUpdateEmailTask *userUpdateEmail(String email);
        UserUpdatePasswordTask *userUpdatePassword(String password);
        UserUpdatePhoneNumberTask *userUpdatePhoneNumberCredential();
        UserUpdateProfileTask *userUpdateProfile(String displayName, String photoUrl);
        String getLanguageCode();
        SendPasswordResetTask *sendPasswordResetEmail(String email);
        void signOut();
        void useEmulator(String host, int port);

        String anonymousProviderId();
        String emailProviderId();
        String phoneProviderId();
        String appleProviderId();
        String facebookProviderId();
        String gameCenterProviderId();
        String gitHubProviderId();
        String googleProviderId();
        String microsoftProviderId();
        String playGamesProviderId();
        String twitterProviderId();
        String yahooProviderId();

        Credential getAppleCredential(String token, String nonce);
        Credential getFacebookCredential(String accessToken);
        void getGameCenterCredential(std::function<void(const Future<Credential> &)> callback);
        Credential getGitHubCredential(String token);
        Credential getGoogleCredential(String idToken, String accessToken);
        Credential getMicrosoftCredential(Array scopes, Dictionary customParameters);
        Credential getPlayGamesCredential(String serverAuthCode);
        Credential getTwitterCredential(String token, String secret);
        Credential getYahooCredential(Array scopes, Dictionary customParameters);
        Credential getOAuthCredential(String providerId, String idToken, String accessToken);
        Credential getOAuthNonceCredential(String providerId, String token, String nonce);
        void getPhoneNumberCredential(String smsCode);

        SignInTask *signInWithApple(String token, String nonce);
        SignInTask *signInWithFacebook(String accessToken);
        SignInTask *signInWithGameCenter();
        bool isGameCenterSignedIn();
        SignInTask *signInWithGitHub(String token);
        SignInTask *signInWithGoogle(String idToken, String accessToken);
        SignInTask *signInWithMicrosoft(Array scopes, Dictionary customParameters);
        SignInTask *signInWithPlayGames(String serverAuthCode);
        SignInTask *signInWithTwitter(String token, String secret);
        SignInTask *signInWithYahoo(Array scopes, Dictionary customParameters);
        SignInTask *signInWithOAuth(String providerId, String idToken, String accessToken);
        SignInTask *signInWithOAuthNonce(String providerId, String token, String nonce);

        UserLinkTask *userLinkWithApple(String token, String nonce);
        UserLinkTask *userLinkWithFacebook(String accessToken);
        UserLinkTask *userLinkWithGameCenter();
        UserLinkTask *userLinkWithGitHub(String token);
        UserLinkTask *userLinkWithGoogle(String idToken, String accessToken);
        UserLinkTask *userLinkWithMicrosoft(Array scopes, Dictionary customParameters);
        UserLinkTask *userLinkWithPlayGames(String serverAuthCode);
        UserLinkTask *userLinkWithTwitter(String token, String secret);
        UserLinkTask *userLinkWithYahoo(Array scopes, Dictionary customParameters);
        UserLinkTask *userLinkWithOAuth(String providerId, String idToken, String accessToken);
        UserLinkTask *userLinkWithOAuthNonce(String providerId, String token, String nonce);
        UserLinkTask *userLinkWithEmail(String email, String password);
        UserLinkTask *userLinkWithPhoneNumber(String smsCode);

        UserReauthenticateTask *reauthenticateWithApple(String token, String nonce);
        UserReauthenticateTask *reauthenticateWithFacebook(String accessToken);
        UserReauthenticateTask *reauthenticateWithGameCenter();
        UserReauthenticateTask *reauthenticateWithGitHub(String token);
        UserReauthenticateTask *reauthenticateWithGoogle(String idToken, String accessToken);
        UserReauthenticateTask *reauthenticateWithMicrosoft(Array scopes, Dictionary customParameters);
        UserReauthenticateTask *reauthenticateWithPlayGames(String serverAuthCode);
        UserReauthenticateTask *reauthenticateWithTwitter(String token, String secret);
        UserReauthenticateTask *reauthenticateWithYahoo(Array scopes, Dictionary customParameters);
        UserReauthenticateTask *reauthenticateWithOAuth(String providerId, String idToken, String accessToken);
        UserReauthenticateTask *reauthenticateWithOAuthNonce(String providerId, String token, String nonce);
        UserReauthenticateTask *reauthenticateWithEmail(String email, String password);
        UserReauthenticateTask *reauthenticateWithPhoneNumber(String smsCode);

        void verifyPhoneNumber(bool resend, String phoneNumber);
        SignInTask *signInWithPhoneNumber(String smsCode);

        void OnAuthStateChanged(Auth *auth) override;
        void OnIdTokenChanged(Auth *auth) override;

        void OnVerificationCompleted(PhoneAuthCredential credential) override;
        void OnVerificationFailed(const std::string &error) override;
        void OnCodeSent(const std::string &verification_id,
                        const PhoneAuthProvider::ForceResendingToken &force_resending_token) override;
        void OnCodeAutoRetrievalTimeOut(const std::string &verification_id) override;
    };
}