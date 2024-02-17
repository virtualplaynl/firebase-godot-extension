#include "GodotFirebaseAuth.h"
#include "conversion.h"

using namespace godot;
using namespace firebase;
using namespace firebase::auth;

GFB_CALLBACK1_CPP(SignInTask, sign_in, Dictionary, DICTIONARY, user_data);
GFB_CALLBACK1_CPP(CreateUserTask, create_user, Dictionary, DICTIONARY, user_data);
GFB_CALLBACK0_CPP(DeleteUserTask, delete_user);
GFB_CALLBACK1_CPP(GetAuthTokenTask, get_auth_token, String, STRING, token);
GFB_CALLBACK1_CPP(UserLinkTask, user_link, Dictionary, DICTIONARY, user_data);
GFB_CALLBACK1_CPP(UserReauthenticateTask, user_reauthenticate, Dictionary, DICTIONARY, user_data);
GFB_CALLBACK0_CPP(UserReloadTask, user_reload);
GFB_CALLBACK0_CPP(UserSendEmailVerificationTask, user_send_email_verification);
GFB_CALLBACK1_CPP(UserUnlinkTask, user_unlink, Dictionary, DICTIONARY, user_data);
GFB_CALLBACK1_CPP(UserUpdateEmailTask, user_update_email, String, STRING, email);
GFB_CALLBACK1_CPP(UserUpdatePasswordTask, user_update_password, String, STRING, password);
GFB_CALLBACK1_CPP(UserUpdatePhoneNumberTask, user_update_phone_number, Dictionary, DICTIONARY, user_data);
GFB_CALLBACK2_CPP(UserUpdateProfileTask, user_update_profile, String, STRING, display_name, String, STRING, photo_url);
GFB_CALLBACK0_CPP(SendPasswordResetTask, send_password_reset);

GodotFirebaseAuth::GodotFirebaseAuth()
{
    auth = nullptr;
    providers = std::map<String, FederatedOAuthProvider>();
    credentials = std::map<String, Credential>();
}

GodotFirebaseAuth::~GodotFirebaseAuth()
{
}

void GodotFirebaseAuth::init(App *app)
{
    if (FirebaseOptions::LogDebug) UtilityFunctions::print("Initializing Firebase Auth...");
    InitResult result;
    auth = Auth::GetAuth(app, &result);
    if (result != kInitResultSuccess)
    {
        UtilityFunctions::push_error("Failed to initialize Firebase Auth, error: ", static_cast<int>(result));
    }
    else
    {
        auth->AddAuthStateListener(this);
        auth->AddIdTokenListener(this);
        
        if (FirebaseOptions::LogDebug) UtilityFunctions::print("Initialized Firebase Auth.");
    }
}

Dictionary convertUserData(User user)
{
    Dictionary userData = Dictionary();
    userData["is_valid"] = user.is_valid();
    if (!user.is_valid())
        return userData;

    Array providerData = Array();
    for (auto &element : user.provider_data())
    {
        Dictionary elementData = Dictionary();
        elementData["display_name"] = element.display_name().c_str();
        elementData["email"] = element.email().c_str();
        elementData["phone_number"] = element.phone_number().c_str();
        elementData["photo_url"] = element.photo_url().c_str();
        elementData["provider_id"] = element.provider_id().c_str();
        elementData["uid"] = element.uid().c_str();
        providerData.append(elementData);
    }
    userData["creation_timestamp"] = user.metadata().creation_timestamp;
    userData["display_name"] = user.display_name().c_str();
    userData["email"] = user.email().c_str();
    userData["is_anonymous"] = user.is_anonymous();
    userData["is_email_verified"] = user.is_email_verified();
    userData["last_sign_in_timestamp"] = user.metadata().last_sign_in_timestamp;
    userData["phone_number"] = user.phone_number().c_str();
    userData["photo_url"] = user.photo_url().c_str();
    userData["provider_data"] = providerData;
    userData["provider_id"] = user.provider_id().c_str();
    userData["uid"] = user.uid().c_str();

    return userData;
}

FederatedOAuthProvider GodotFirebaseAuth::registerProvider(String provider, Array scopes, Dictionary customParameters)
{
    std::string providerId = std::string(provider.utf8().get_data());

    std::map<std::string, std::string> customParameters_map = std::map<std::string, std::string>();
    Array parms = customParameters.keys();
    for (size_t s = 0; s < parms.size(); s++)
        customParameters_map[std::string(((String)parms[s]).utf8().get_data())]
            = std::string(((String)customParameters[parms[s]]).utf8().get_data());
    FederatedOAuthProvider providerReg = FederatedOAuthProvider(FederatedOAuthProviderData(providerId, arrayToStringVector(scopes), customParameters_map));

    providers[provider] = providerReg;

    return providerReg;
}

SignInTask *GodotFirebaseAuth::signInAnonymously()
{
    SignInTask *task = memnew(SignInTask);
    auth->SignInAnonymously()
        .OnCompletion([task, this](const Future<AuthResult> &completedTask)
                      {
            if(completedTask.error() == 0) {
                const AuthResult *loginResult = completedTask.result();
                credentials.insert_or_assign(anonymousProviderId(), loginResult->credential);
                if (loginResult)
                    task->completed(convertUserData(loginResult->user));
                else
                    task->error(-2, "Unable to sign in anonymously");
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            } });
    return task;
}

Credential GodotFirebaseAuth::getEmailCredential(String email, String password)
{
    return EmailAuthProvider::GetCredential(email.utf8().get_data(), password.utf8().get_data());
}

SignInTask *GodotFirebaseAuth::signInWithEmailAndPassword(String email, String password)
{
    Credential credential = getEmailCredential(email, password);
    return signInWithCredential(credential);
}

SignInTask *GodotFirebaseAuth::signInWithCredentialFrom(String providerId)
{
    Credential credential = credentials[providerId];
    return signInWithCredential(credential);
}
SignInTask *GodotFirebaseAuth::signInWithCredential(Credential credential)
{
    SignInTask *task = memnew(SignInTask);
    auth->SignInAndRetrieveDataWithCredential(credential)
        .OnCompletion([task](const Future<AuthResult> &completedTask)
                      {
            if(completedTask.error() == 0) {
                const AuthResult *loginResult = completedTask.result();
                if (loginResult)
                    task->completed(convertUserData(loginResult->user));
                else
                    task->error(-1, "Unable to sign in with credential");
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            } });
    return task;
}

SignInTask *GodotFirebaseAuth::signInWithCustomToken(String token)
{
    SignInTask *task = memnew(SignInTask);
    auth->SignInWithCustomToken(token.utf8().get_data())
        .OnCompletion([task](const Future<AuthResult> &completedTask)
                      {
            if(completedTask.error() == 0) {
                const AuthResult *loginResult = completedTask.result();
                if (loginResult)
                    task->completed(convertUserData(loginResult->user));
                else
                    task->error(-1, "Unable to sign in with custom token");
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            } });
    return task;
}
SignInTask *GodotFirebaseAuth::signInWithProviderId(String providerId)
{
    FederatedAuthProvider *provider = &(providers[providerId]);
    return signInWithProvider(provider, providerId);
}
SignInTask *GodotFirebaseAuth::signInWithProvider(FederatedAuthProvider *provider, String providerId)
{
    SignInTask *task = memnew(SignInTask);
    auth->SignInWithProvider(provider)
        .OnCompletion([task, this, providerId](const Future<AuthResult> &completedTask)
        {
            if(completedTask.error() == 0) {
                const AuthResult *loginResult = completedTask.result();
                credentials.insert_or_assign(providerId, loginResult->credential);
                if (loginResult)
                    task->completed(convertUserData(loginResult->user));
                else
                    task->error(-1, "Unable to sign in with provider");
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}

void GodotFirebaseAuth::setLanguageCode(String languageCode)
{
    auth->set_language_code(languageCode.utf8().get_data());
}
CreateUserTask *GodotFirebaseAuth::createUserWithEmailAndPassword(String email, String password)
{
    CreateUserTask *task = memnew(CreateUserTask);
    auth->CreateUserWithEmailAndPassword(email.utf8().ptr(), password.utf8().ptr())
        .OnCompletion([task](const Future<AuthResult> &completedTask)
        {
            if(completedTask.error() == 0) {
                const AuthResult *loginResult = completedTask.result();
                if (loginResult)
                    task->completed(convertUserData(loginResult->user));
                else
                    task->error(-1, "Unable to create user");
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
Dictionary GodotFirebaseAuth::getUser()
{
    return convertUserData(auth->current_user());
}
DeleteUserTask *GodotFirebaseAuth::userDelete()
{
    DeleteUserTask *task = memnew(DeleteUserTask);
    auth->current_user().Delete()
        .OnCompletion([task](const Future<void> &completedTask)
        {
            if(completedTask.error() == 0)
                task->completed();
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
GetAuthTokenTask *GodotFirebaseAuth::userGetToken(bool forceRefresh)
{
    GetAuthTokenTask *task = memnew(GetAuthTokenTask);
    auth->current_user().GetToken(forceRefresh)
        .OnCompletion([task](Future<std::string> completedTask)
        {
            if(completedTask.error() == 0) {
                String token = String(completedTask.result()->c_str());
                task->completed(token);
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
UserLinkTask *GodotFirebaseAuth::userLinkWithCredentialFrom(String providerId)
{
    Credential credential = credentials[providerId];
    return userLinkWithCredential(credential);
}
UserLinkTask *GodotFirebaseAuth::userLinkWithCredential(Credential credential)
{
    UserLinkTask *task = memnew(UserLinkTask);
    auth->current_user().LinkWithCredential(credential)
        .OnCompletion([task](const Future<AuthResult> &completedTask)
        {
            if(completedTask.error() == 0) {
                const AuthResult *linkResult = completedTask.result();
                if (linkResult)
                    task->completed(convertUserData(linkResult->user));
                else
                    task->error(-1, "Unable to create user");
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
UserLinkTask *GodotFirebaseAuth::userLinkWithProviderId(String providerId)
{
    FederatedAuthProvider *provider = &(providers[providerId]);
    return userLinkWithProvider(provider, providerId);
}
UserLinkTask *GodotFirebaseAuth::userLinkWithProvider(FederatedAuthProvider *provider, String providerId)
{
    UserLinkTask *task = memnew(UserLinkTask);
    auth->current_user().LinkWithProvider(provider)
        .OnCompletion([task, this, providerId](const Future<AuthResult> &completedTask)
        {
            if(completedTask.error() == 0) {
                const AuthResult *linkResult = completedTask.result();
                if (linkResult)
                {
                    credentials.insert_or_assign(providerId, completedTask.result()->credential);
                    task->completed(convertUserData(linkResult->user));
                }
                else task->error(-1, "Unable to create user");
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
UserReauthenticateTask *GodotFirebaseAuth::userReauthenticateWithCredentialFrom(String providerId)
{
    Credential credential = credentials[providerId];
    return userReauthenticateWithCredential(credential);
}
UserReauthenticateTask *GodotFirebaseAuth::userReauthenticateWithCredential(Credential credential)
{
    UserReauthenticateTask *task = memnew(UserReauthenticateTask);
    auth->current_user().ReauthenticateAndRetrieveData(credential)
        .OnCompletion([task](const Future<AuthResult> &completedTask)
        {
            if(completedTask.error() == 0) {
                const AuthResult *loginResult = completedTask.result();
                if (loginResult)
                    task->completed(convertUserData(loginResult->user));
                else
                    task->error(-1, "Unable to create user");
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
UserReauthenticateTask *GodotFirebaseAuth::userReauthenticateWithProviderId(String providerId)
{
    FederatedAuthProvider *provider = &(providers[providerId]);
    return userReauthenticateWithProvider(provider, providerId);
}
UserReauthenticateTask *GodotFirebaseAuth::userReauthenticateWithProvider(FederatedAuthProvider *provider, String providerId)
{
    UserReauthenticateTask *task = memnew(UserReauthenticateTask);
    auth->current_user().ReauthenticateWithProvider(provider)
        .OnCompletion([task, this, providerId](const Future<AuthResult> &completedTask)
        {
            if(completedTask.error() == 0) {
                const AuthResult *loginResult = completedTask.result();
                if (loginResult)
                {
                    credentials.insert_or_assign(providerId, completedTask.result()->credential);
                    task->completed(convertUserData(loginResult->user));
                }
                else
                    task->error(-1, "Unable to create user");
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
UserReloadTask *GodotFirebaseAuth::userReload()
{
    UserReloadTask *task = memnew(UserReloadTask);
    auth->current_user().Reload()
        .OnCompletion([task](const Future<void> &completedTask)
        {
            if(completedTask.error() == 0)
                task->completed();
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
UserSendEmailVerificationTask *GodotFirebaseAuth::userSendEmailVerification()
{
    UserSendEmailVerificationTask *task = memnew(UserSendEmailVerificationTask);
    auth->current_user().SendEmailVerification()
        .OnCompletion([task](const Future<void> &completedTask)
        {
            if(completedTask.error() == 0)
                task->completed();
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
UserUnlinkTask *GodotFirebaseAuth::userUnlink(String providerId)
{
    UserUnlinkTask *task = memnew(UserUnlinkTask);
    auth->current_user().Unlink(providerId.utf8().get_data())
        .OnCompletion([task](const Future<AuthResult> &completedTask)
        {
            if(completedTask.error() == 0) {
                const AuthResult *linkResult = completedTask.result();
                if (linkResult)
                    task->completed(convertUserData(linkResult->user));
                else
                    task->error(-1, "Unable to create user");
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
UserUpdateEmailTask *GodotFirebaseAuth::userUpdateEmail(String email)
{
    UserUpdateEmailTask *task = memnew(UserUpdateEmailTask);
    auth->current_user().UpdateEmail(email.utf8().get_data())
        .OnCompletion([task, email](const Future<void> &completedTask)
        {
            if(completedTask.error() == 0)
                task->completed(email);
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
UserUpdatePasswordTask *GodotFirebaseAuth::userUpdatePassword(String password)
{
    UserUpdatePasswordTask *task = memnew(UserUpdatePasswordTask);
    auth->current_user().UpdatePassword(password.utf8().get_data())
        .OnCompletion([task, password](const Future<void> &completedTask)
        {
            if(completedTask.error() == 0)
                task->completed(password);
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
UserUpdatePhoneNumberTask *GodotFirebaseAuth::userUpdatePhoneNumberCredential()
{
    UserUpdatePhoneNumberTask *task = memnew(UserUpdatePhoneNumberTask);
    auth->current_user().UpdatePhoneNumberCredential(PhoneAuthCredential())
        .OnCompletion([task](const Future<User> &completedTask)
        {
            if(completedTask.error() == 0) {
                const User *user = completedTask.result();
                if (user)
                    task->completed(convertUserData(*user));
                else
                    task->error(-1, "Unable to create user");
            }
            else
            {
                task->error(completedTask.error(), completedTask.error_message());
            }
        });
    return task;
}
UserUpdateProfileTask *GodotFirebaseAuth::userUpdateProfile(String displayName, String photoUrl)
{
    User::UserProfile profile = User::UserProfile();
    if (displayName.length() > 0) profile.display_name = displayName.utf8().get_data();
    if (photoUrl.length() > 0) profile.photo_url = photoUrl.utf8().get_data();
    UserUpdateProfileTask *task = memnew(UserUpdateProfileTask);
    auth->current_user().UpdateUserProfile(profile)
        .OnCompletion([task, displayName, photoUrl](const Future<void> &completedTask)
        {
            if(completedTask.error() == 0)
                task->completed(displayName, photoUrl);
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
String GodotFirebaseAuth::getLanguageCode()
{
    return String(auth->language_code().c_str());
}
SendPasswordResetTask *GodotFirebaseAuth::sendPasswordResetEmail(String email)
{
    SendPasswordResetTask *task = memnew(SendPasswordResetTask);
    auth->SendPasswordResetEmail(email.utf8().get_data())
        .OnCompletion([task](const Future<void> &completedTask)
        {
            if(completedTask.error() == 0)
                task->completed();
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
void GodotFirebaseAuth::signOut()
{
    auth->SignOut();
}
void GodotFirebaseAuth::useEmulator(String host, int port)
{
    auth->UseEmulator(host.utf8().get_data(), port);
}


#pragma region AuthProviders

String GodotFirebaseAuth::anonymousProviderId() { return "anonymous"; }
String GodotFirebaseAuth::emailProviderId() { return EmailAuthProvider::kProviderId; }
String GodotFirebaseAuth::phoneProviderId() { return PhoneAuthProvider::kProviderId; }
String GodotFirebaseAuth::appleProviderId() { return "apple.com"; }
String GodotFirebaseAuth::facebookProviderId() { return FacebookAuthProvider::kProviderId; }
String GodotFirebaseAuth::gameCenterProviderId() { return GameCenterAuthProvider::kProviderId; }
String GodotFirebaseAuth::gitHubProviderId() { return GitHubAuthProvider::kProviderId; }
String GodotFirebaseAuth::googleProviderId() { return GoogleAuthProvider::kProviderId; }
String GodotFirebaseAuth::microsoftProviderId() { return MicrosoftAuthProvider::kProviderId; }
String GodotFirebaseAuth::playGamesProviderId() { return PlayGamesAuthProvider::kProviderId; }
String GodotFirebaseAuth::twitterProviderId() { return TwitterAuthProvider::kProviderId; }
String GodotFirebaseAuth::yahooProviderId() { return YahooAuthProvider::kProviderId; }

Credential GodotFirebaseAuth::getAppleCredential(String token, String nonce)
{
    return getOAuthNonceCredential(appleProviderId(), token, nonce);
}
Credential GodotFirebaseAuth::getFacebookCredential(String accessToken)
{
    Credential credential = FacebookAuthProvider::GetCredential(accessToken.utf8().get_data());
    credentials.insert_or_assign(facebookProviderId(), credential);
    return credential;
}
void GodotFirebaseAuth::getGameCenterCredential(std::function<void(const Future<Credential> &)>callback)
{
    GameCenterAuthProvider::GetCredential().OnCompletion([this, callback](Future<Credential> completedTask)
    {
        if(completedTask.error() == 0)
        {
            credentials.insert_or_assign(gameCenterProviderId(), *completedTask.result());
        }
        callback(completedTask);
    });
}
Credential GodotFirebaseAuth::getGitHubCredential(String token)
{
    Credential credential = GitHubAuthProvider::GetCredential(token.utf8().get_data());
    credentials.insert_or_assign(gitHubProviderId(), credential);
    return credential;
}
Credential GodotFirebaseAuth::getGoogleCredential(String idToken, String accessToken)
{
    Credential credential = GoogleAuthProvider::GetCredential(idToken.utf8().get_data(), accessToken.utf8().get_data());
    credentials.insert_or_assign(googleProviderId(), credential);
    return credential;
}
Credential GodotFirebaseAuth::getPlayGamesCredential(String serverAuthCode)
{
    Credential credential = PlayGamesAuthProvider::GetCredential(serverAuthCode.utf8().get_data());
    credentials.insert_or_assign(playGamesProviderId(), credential);
    return credential;
}
Credential GodotFirebaseAuth::getTwitterCredential(String token, String secret)
{
    Credential credential = TwitterAuthProvider::GetCredential(token.utf8().get_data(), secret.utf8().get_data());
    credentials.insert_or_assign(twitterProviderId(), credential);
    return credential;
}
Credential GodotFirebaseAuth::getOAuthCredential(String providerId, String idToken, String accessToken)
{
    Credential credential = OAuthProvider::GetCredential(providerId.utf8().get_data(), idToken.utf8().get_data(), accessToken.utf8().get_data());
    credentials.insert_or_assign(providerId, credential);
    return credential;
}
Credential GodotFirebaseAuth::getOAuthNonceCredential(String providerId, String token, String nonce)
{
    Credential credential = OAuthProvider::GetCredential(providerId.utf8().get_data(), token.utf8().get_data(), nonce.utf8().get_data(), nullptr);
    credentials.insert_or_assign(providerId, credential);
    return credential;
}
void GodotFirebaseAuth::getPhoneNumberCredential(String smsCode)
{
    PhoneAuthProvider &phoneProvider = PhoneAuthProvider::GetInstance(auth);
    phoneCredential = phoneProvider.GetCredential(phoneVerificationCode.c_str(), smsCode.utf8().get_data());
}

SignInTask *GodotFirebaseAuth::signInWithApple(String token, String nonce)
{
    return signInWithOAuthNonce(appleProviderId(), token, nonce);
}
SignInTask *GodotFirebaseAuth::signInWithFacebook(String accessToken)
{
    return signInWithCredential(getFacebookCredential(accessToken));
}
SignInTask *GodotFirebaseAuth::signInWithGameCenter()
{
    SignInTask *task = memnew(SignInTask);
    GameCenterAuthProvider::GetCredential().OnCompletion([task, this](Future<Credential> completedTask)
                                                         {
        if(completedTask.error() == 0)
        {
            credentials.insert_or_assign(gameCenterProviderId(), *completedTask.result());
            signInWithCredential(*completedTask.result())->forward(task);
        }
        else
            task->error(completedTask.error(), completedTask.error_message()); });
    return task;
}
bool GodotFirebaseAuth::isGameCenterSignedIn()
{
    return GameCenterAuthProvider::IsPlayerAuthenticated();
}
SignInTask *GodotFirebaseAuth::signInWithGitHub(String token)
{
    return signInWithCredential(getGitHubCredential(token));
}
SignInTask *GodotFirebaseAuth::signInWithGoogle(String idToken, String accessToken)
{
    return signInWithCredential(getGoogleCredential(idToken, accessToken));
}
SignInTask *GodotFirebaseAuth::signInWithMicrosoft(Array scopes, Dictionary customParameters)
{
    FederatedOAuthProvider provider = registerProvider(MicrosoftAuthProvider::kProviderId, scopes, customParameters);
    return signInWithProvider(&provider, microsoftProviderId());
}
SignInTask *GodotFirebaseAuth::signInWithPlayGames(String serverAuthCode)
{
    return signInWithCredential(getPlayGamesCredential(serverAuthCode));
}
SignInTask *GodotFirebaseAuth::signInWithTwitter(String token, String secret)
{
    return signInWithCredential(getTwitterCredential(token, secret));
}
SignInTask *GodotFirebaseAuth::signInWithYahoo(Array scopes, Dictionary customParameters)
{
    FederatedOAuthProvider provider = registerProvider(YahooAuthProvider::kProviderId, scopes, customParameters);
    return signInWithProvider(&provider, yahooProviderId());
}
SignInTask *GodotFirebaseAuth::signInWithOAuth(String providerId, String idToken, String accessToken)
{
    return signInWithCredential(getOAuthCredential(providerId, idToken, accessToken));
}
SignInTask *GodotFirebaseAuth::signInWithOAuthNonce(String providerId, String token, String nonce)
{
    return signInWithCredential(getOAuthNonceCredential(providerId, token, nonce));
}

void GodotFirebaseAuth::verifyPhoneNumber(bool resend, String phoneNumber)
{
    PhoneAuthOptions options = PhoneAuthOptions();
    if (resend)
        options.force_resending_token = &phoneResendingToken;
    options.phone_number = std::string(phoneNumber.utf8().get_data());
    options.timeout_milliseconds = 60000;
    PhoneAuthProvider &phoneProvider = PhoneAuthProvider::GetInstance(auth);
    phoneProvider.VerifyPhoneNumber(options, this);
}
SignInTask *GodotFirebaseAuth::signInWithPhoneNumber(String smsCode)
{
    getPhoneNumberCredential(smsCode);
    return signInWithCredential(phoneCredential);
}

UserLinkTask *GodotFirebaseAuth::userLinkWithApple(String token, String nonce)
{
    return userLinkWithOAuthNonce(appleProviderId(), token, nonce);
}
UserLinkTask *GodotFirebaseAuth::userLinkWithFacebook(String accessToken)
{
    return userLinkWithCredential(getFacebookCredential(accessToken));
}
UserLinkTask *GodotFirebaseAuth::userLinkWithGameCenter()
{
    UserLinkTask *task = memnew(UserLinkTask);
    getGameCenterCredential([task, this](Future<Credential> completedTask)
        {
            if(completedTask.error() == 0)
                userLinkWithCredential(*completedTask.result())->forward(task);
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
UserLinkTask *GodotFirebaseAuth::userLinkWithGitHub(String token)
{
    return userLinkWithCredential(getGitHubCredential(token));
}
UserLinkTask *GodotFirebaseAuth::userLinkWithGoogle(String idToken, String accessToken)
{
    return userLinkWithCredential(getGoogleCredential(idToken, accessToken));
}
UserLinkTask *GodotFirebaseAuth::userLinkWithMicrosoft(Array scopes, Dictionary customParameters)
{
    FederatedOAuthProvider provider = registerProvider(MicrosoftAuthProvider::kProviderId, scopes, customParameters);
    return userLinkWithProvider(&provider, microsoftProviderId());
}
UserLinkTask *GodotFirebaseAuth::userLinkWithPlayGames(String serverAuthCode)
{
    return userLinkWithCredential(getPlayGamesCredential(serverAuthCode));
}
UserLinkTask * GodotFirebaseAuth::userLinkWithTwitter(String token, String secret)
{
    return userLinkWithCredential(getTwitterCredential(token, secret));
}
UserLinkTask *GodotFirebaseAuth::userLinkWithYahoo(Array scopes, Dictionary customParameters)
{
    FederatedOAuthProvider provider = registerProvider(YahooAuthProvider::kProviderId, scopes, customParameters);
    return userLinkWithProvider(&provider, yahooProviderId());
}
UserLinkTask *GodotFirebaseAuth::userLinkWithOAuth(String providerId, String idToken, String accessToken)
{
    return userLinkWithCredential(getOAuthCredential(providerId, idToken, accessToken));
}
UserLinkTask *GodotFirebaseAuth::userLinkWithOAuthNonce(String providerId, String token, String nonce)
{
    return userLinkWithCredential(getOAuthNonceCredential(providerId, token, nonce));
}
UserLinkTask *GodotFirebaseAuth::userLinkWithEmail(String email, String password)
{
    return userLinkWithCredential(getEmailCredential(email, password));
}
UserLinkTask *GodotFirebaseAuth::userLinkWithPhoneNumber(String smsCode)
{
    getPhoneNumberCredential(smsCode);
    return userLinkWithCredential(phoneCredential);
}


UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithApple(String token, String nonce)
{
    return reauthenticateWithOAuthNonce(appleProviderId(), token, nonce);
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithFacebook(String accessToken)
{
    return userReauthenticateWithCredential(getFacebookCredential(accessToken));
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithGameCenter()
{
    UserReauthenticateTask *task = memnew(UserReauthenticateTask);
    getGameCenterCredential([task, this](Future<Credential> completedTask)
        {
            if(completedTask.error() == 0)
                userReauthenticateWithCredential(*completedTask.result())->forward(task);
            else
                task->error(completedTask.error(), completedTask.error_message());
        });
    return task;
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithGitHub(String token)
{
    return userReauthenticateWithCredential(getGitHubCredential(token));
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithGoogle(String idToken, String accessToken)
{
    return userReauthenticateWithCredential(getGoogleCredential(idToken, accessToken));
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithMicrosoft(Array scopes, Dictionary customParameters)
{
    FederatedOAuthProvider provider = registerProvider(MicrosoftAuthProvider::kProviderId, scopes, customParameters);
    return userReauthenticateWithProvider(&provider, microsoftProviderId());
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithPlayGames(String serverAuthCode)
{
    return userReauthenticateWithCredential(getPlayGamesCredential(serverAuthCode));
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithTwitter(String token, String secret)
{
    return userReauthenticateWithCredential(getTwitterCredential(token, secret));
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithYahoo(Array scopes, Dictionary customParameters)
{
    FederatedOAuthProvider provider = registerProvider(YahooAuthProvider::kProviderId, scopes, customParameters);
    return userReauthenticateWithProvider(&provider, yahooProviderId());
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithOAuth(String providerId, String idToken, String accessToken)
{
    return userReauthenticateWithCredential(getOAuthCredential(providerId, idToken, accessToken));
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithOAuthNonce(String providerId, String token, String nonce)
{
    return userReauthenticateWithCredential(getOAuthNonceCredential(providerId, token, nonce));
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithEmail(String email, String password)
{
    return userReauthenticateWithCredential(getEmailCredential(email, password));
}
UserReauthenticateTask *GodotFirebaseAuth::reauthenticateWithPhoneNumber(String smsCode)
{
    getPhoneNumberCredential(smsCode);
    return userReauthenticateWithCredential(phoneCredential);
}

#pragma endregion AuthProviders

void GodotFirebaseAuth::OnAuthStateChanged(Auth *auth)
{
    if(!auth->current_user().is_valid())
    {
        credentials.clear();
    }
    Dictionary credentialsDict = Dictionary();
    for (auto credential : credentials)
    {
        if(credential.second.is_valid())
            credentialsDict[credential.first] = credential.second.provider().c_str();
    }
    emit_signal("auth_state_changed", convertUserData(auth->current_user()), credentialsDict);
}
void GodotFirebaseAuth::OnIdTokenChanged(Auth *auth)
{
    auth->current_user().GetToken(false).OnCompletion([this](Future<std::string> completedTask)
        {
            if(completedTask.error() == 0) {
                String token = String(completedTask.result()->c_str());
                emit_signal("id_token_changed", token);
            }
        });
}

void GodotFirebaseAuth::OnVerificationCompleted(PhoneAuthCredential credential)
{
    emit_signal("phone_verification_completed");
}
void GodotFirebaseAuth::OnVerificationFailed(const std::string &error)
{
    emit_signal("phone_verification_failed", error.c_str());
}
void GodotFirebaseAuth::OnCodeSent(const std::string &verification_id,
                                   const PhoneAuthProvider::ForceResendingToken &force_resending_token)
{
    phoneVerificationCode = verification_id;
    phoneResendingToken = force_resending_token;
    emit_signal("verification_sms_sent");
}
void GodotFirebaseAuth::OnCodeAutoRetrievalTimeOut(const std::string &verification_id)
{
    phoneVerificationCode = verification_id;
    emit_signal("auto_sms_failed");
}

void GodotFirebaseAuth::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("sign_in_with_email_and_password", "email", "password"), &GodotFirebaseAuth::signInWithEmailAndPassword);
    ClassDB::bind_method(D_METHOD("sign_in_anonymously"), &GodotFirebaseAuth::signInAnonymously);
    ClassDB::bind_method(D_METHOD("sign_in_with_credential_from", "provider_id"), &GodotFirebaseAuth::signInWithCredentialFrom);
    ClassDB::bind_method(D_METHOD("sign_in_with_custom_token", "token"), &GodotFirebaseAuth::signInWithCustomToken);
    ClassDB::bind_method(D_METHOD("sign_in_with_provider", "provider_id"), &GodotFirebaseAuth::signInWithProviderId);
    ClassDB::bind_method(D_METHOD("set_language_code", "language_code"), &GodotFirebaseAuth::setLanguageCode);
    ClassDB::bind_method(D_METHOD("create_user_with_email_and_password", "email", "password"), &GodotFirebaseAuth::createUserWithEmailAndPassword);
    ClassDB::bind_method(D_METHOD("get_user"), &GodotFirebaseAuth::getUser);
    ClassDB::bind_method(D_METHOD("user_delete"), &GodotFirebaseAuth::userDelete);
    ClassDB::bind_method(D_METHOD("user_get_token", "force_refresh"), &GodotFirebaseAuth::userGetToken);
    ClassDB::bind_method(D_METHOD("user_link_with_credential_from", "provider_id"), &GodotFirebaseAuth::userLinkWithCredentialFrom);
    ClassDB::bind_method(D_METHOD("user_link_with_provider", "provider_id"), &GodotFirebaseAuth::userLinkWithProviderId);
    ClassDB::bind_method(D_METHOD("user_reauthenticate_with_credential_from", "provider_id"), &GodotFirebaseAuth::userReauthenticateWithCredentialFrom);
    ClassDB::bind_method(D_METHOD("user_reauthenticate_with_provider", "provider_id"), &GodotFirebaseAuth::userReauthenticateWithProviderId);
    ClassDB::bind_method(D_METHOD("user_reload"), &GodotFirebaseAuth::userReload);
    ClassDB::bind_method(D_METHOD("user_send_email_verification"), &GodotFirebaseAuth::userSendEmailVerification);
    ClassDB::bind_method(D_METHOD("user_unlink", "provider_id"), &GodotFirebaseAuth::userUnlink);
    ClassDB::bind_method(D_METHOD("user_update_email", "email"), &GodotFirebaseAuth::userUpdateEmail);
    ClassDB::bind_method(D_METHOD("user_update_password", "password"), &GodotFirebaseAuth::userUpdatePassword);
    ClassDB::bind_method(D_METHOD("user_update_phone_number_credential"), &GodotFirebaseAuth::userUpdatePhoneNumberCredential);
    ClassDB::bind_method(D_METHOD("user_update_profile", "display_name", "photo_url"), &GodotFirebaseAuth::userUpdateProfile);
    ClassDB::bind_method(D_METHOD("get_language_code"), &GodotFirebaseAuth::getLanguageCode);
    ClassDB::bind_method(D_METHOD("send_password_reset_email", "email"), &GodotFirebaseAuth::sendPasswordResetEmail);
    ClassDB::bind_method(D_METHOD("sign_out"), &GodotFirebaseAuth::signOut);
    ClassDB::bind_method(D_METHOD("use_emulator", "host", "port"), &GodotFirebaseAuth::useEmulator);

    ClassDB::bind_method(D_METHOD("get_email_provider_id"), &GodotFirebaseAuth::emailProviderId);
    ClassDB::bind_method(D_METHOD("get_phone_provider_id"), &GodotFirebaseAuth::phoneProviderId);
    ClassDB::bind_method(D_METHOD("get_apple_provider_id"), &GodotFirebaseAuth::appleProviderId);
    ClassDB::bind_method(D_METHOD("get_facebook_provider_id"), &GodotFirebaseAuth::facebookProviderId);
    ClassDB::bind_method(D_METHOD("get_gamecenter_provider_id"), &GodotFirebaseAuth::gameCenterProviderId);
    ClassDB::bind_method(D_METHOD("get_github_provider_id"), &GodotFirebaseAuth::gitHubProviderId);
    ClassDB::bind_method(D_METHOD("get_google_provider_id"), &GodotFirebaseAuth::googleProviderId);
    ClassDB::bind_method(D_METHOD("get_microsoft_provider_id"), &GodotFirebaseAuth::microsoftProviderId);
    ClassDB::bind_method(D_METHOD("get_playgames_provider_id"), &GodotFirebaseAuth::playGamesProviderId);
    ClassDB::bind_method(D_METHOD("get_twitter_provider_id"), &GodotFirebaseAuth::twitterProviderId);
    ClassDB::bind_method(D_METHOD("get_x_provider_id"), &GodotFirebaseAuth::twitterProviderId);
    ClassDB::bind_method(D_METHOD("get_yahoo_provider_id"), &GodotFirebaseAuth::yahooProviderId);

    ClassDB::bind_method(D_METHOD("sign_in_with_apple", "token", "nonce"), &GodotFirebaseAuth::signInWithApple);
    ClassDB::bind_method(D_METHOD("sign_in_with_facebook", "accessToken"), &GodotFirebaseAuth::signInWithFacebook);
    ClassDB::bind_method(D_METHOD("is_gamecenter_signed_in"), &GodotFirebaseAuth::isGameCenterSignedIn);
    ClassDB::bind_method(D_METHOD("sign_in_with_gamecenter"), &GodotFirebaseAuth::signInWithGameCenter);
    ClassDB::bind_method(D_METHOD("sign_in_with_github", "token"), &GodotFirebaseAuth::signInWithGitHub);
    ClassDB::bind_method(D_METHOD("sign_in_with_google", "idToken", "accessToken"), &GodotFirebaseAuth::signInWithGoogle);
    ClassDB::bind_method(D_METHOD("sign_in_with_microsoft", "scopes", "customParameters"), &GodotFirebaseAuth::signInWithMicrosoft);
    ClassDB::bind_method(D_METHOD("sign_in_with_playgames", "serverAuthCode"), &GodotFirebaseAuth::signInWithPlayGames);
    ClassDB::bind_method(D_METHOD("sign_in_with_twitter", "token", "secret"), &GodotFirebaseAuth::signInWithTwitter);
    ClassDB::bind_method(D_METHOD("sign_in_with_x", "token", "secret"), &GodotFirebaseAuth::signInWithTwitter);
    ClassDB::bind_method(D_METHOD("sign_in_with_yahoo", "scopes", "customParameters"), &GodotFirebaseAuth::signInWithYahoo);
    ClassDB::bind_method(D_METHOD("sign_in_with_oauth", "providerId", "idToken", "accessToken"), &GodotFirebaseAuth::signInWithOAuth);
    ClassDB::bind_method(D_METHOD("sign_in_with_oauth_nonce", "providerId", "token", "nonce"), &GodotFirebaseAuth::signInWithOAuthNonce);
    ClassDB::bind_method(D_METHOD("verify_phone_number", "resend", "phone_number"), &GodotFirebaseAuth::verifyPhoneNumber);
    ClassDB::bind_method(D_METHOD("sign_in_with_phone_number", "sms_code"), &GodotFirebaseAuth::signInWithPhoneNumber);

    ClassDB::bind_method(D_METHOD("user_link_with_apple", "token", "nonce"), &GodotFirebaseAuth::userLinkWithApple);
    ClassDB::bind_method(D_METHOD("user_link_with_facebook", "accessToken"), &GodotFirebaseAuth::userLinkWithFacebook);
    ClassDB::bind_method(D_METHOD("user_link_with_gamecenter"), &GodotFirebaseAuth::userLinkWithGameCenter);
    ClassDB::bind_method(D_METHOD("user_link_with_github", "token"), &GodotFirebaseAuth::userLinkWithGitHub);
    ClassDB::bind_method(D_METHOD("user_link_with_google", "idToken", "accessToken"), &GodotFirebaseAuth::userLinkWithGoogle);
    ClassDB::bind_method(D_METHOD("user_link_with_microsoft", "scopes", "customParameters"), &GodotFirebaseAuth::userLinkWithMicrosoft);
    ClassDB::bind_method(D_METHOD("user_link_with_playgames", "serverAuthCode"), &GodotFirebaseAuth::userLinkWithPlayGames);
    ClassDB::bind_method(D_METHOD("user_link_with_twitter", "token", "secret"), &GodotFirebaseAuth::userLinkWithTwitter);
    ClassDB::bind_method(D_METHOD("user_link_with_x", "token", "secret"), &GodotFirebaseAuth::userLinkWithTwitter);
    ClassDB::bind_method(D_METHOD("user_link_with_yahoo", "scopes", "customParameters"), &GodotFirebaseAuth::userLinkWithYahoo);
    ClassDB::bind_method(D_METHOD("user_link_with_oauth", "providerId", "idToken", "accessToken"), &GodotFirebaseAuth::userLinkWithOAuth);
    ClassDB::bind_method(D_METHOD("user_link_with_oauth_nonce", "providerId", "token", "nonce"), &GodotFirebaseAuth::userLinkWithOAuthNonce);
    ClassDB::bind_method(D_METHOD("user_link_with_email", "email", "password"), &GodotFirebaseAuth::userLinkWithEmail);
    ClassDB::bind_method(D_METHOD("user_link_with_phone_number", "sms_code"), &GodotFirebaseAuth::userLinkWithPhoneNumber);

    ClassDB::bind_method(D_METHOD("reauthenticate_with_apple", "token", "nonce"), &GodotFirebaseAuth::reauthenticateWithApple);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_facebook", "accessToken"), &GodotFirebaseAuth::reauthenticateWithFacebook);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_gamecenter"), &GodotFirebaseAuth::reauthenticateWithGameCenter);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_github", "token"), &GodotFirebaseAuth::reauthenticateWithGitHub);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_google", "idToken", "accessToken"), &GodotFirebaseAuth::reauthenticateWithGoogle);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_microsoft", "scopes", "customParameters"), &GodotFirebaseAuth::reauthenticateWithMicrosoft);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_playgames", "serverAuthCode"), &GodotFirebaseAuth::reauthenticateWithPlayGames);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_twitter", "token", "secret"), &GodotFirebaseAuth::reauthenticateWithTwitter);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_x", "token", "secret"), &GodotFirebaseAuth::reauthenticateWithTwitter);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_yahoo", "scopes", "customParameters"), &GodotFirebaseAuth::reauthenticateWithYahoo);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_oauth", "providerId", "idToken", "accessToken"), &GodotFirebaseAuth::reauthenticateWithOAuth);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_oauth_nonce", "providerId", "token", "nonce"), &GodotFirebaseAuth::reauthenticateWithOAuthNonce);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_email", "email", "password"), &GodotFirebaseAuth::reauthenticateWithEmail);
    ClassDB::bind_method(D_METHOD("reauthenticate_with_phone_number", "sms_code"), &GodotFirebaseAuth::reauthenticateWithPhoneNumber);

    ADD_SIGNAL(MethodInfo("auth_state_changed", PropertyInfo(Variant::DICTIONARY, "user_data"), PropertyInfo(Variant::DICTIONARY, "providers")));
    ADD_SIGNAL(MethodInfo("id_token_changed", PropertyInfo(Variant::STRING, "token")));
    ADD_SIGNAL(MethodInfo("phone_verification_completed"));
    ADD_SIGNAL(MethodInfo("phone_verification_failed", PropertyInfo(Variant::STRING, "error")));
    ADD_SIGNAL(MethodInfo("verification_sms_sent"));
    ADD_SIGNAL(MethodInfo("auto_sms_failed"));
}
