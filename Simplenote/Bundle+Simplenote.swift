import Foundation

/// Bundle: Simplenote Methods
///
extension Bundle {

    /// Returns the Bundle Short Version String.
    ///
    @objc
    var shortVersionString: String {
        let version = infoDictionary?["CFBundleShortVersionString"] as? String
        return version ?? ""
    }

    var rootBundleIdentifier: String {
        guard isAppExtensionBundle,
              bundleURL.path.components(separatedBy: "/").contains(where: { $0.hasSuffix(Constants.appSuffix) }) else {
            return bundleIdentifier ?? Constants.defaultBundleID
        }

        var url = bundleURL

        for component in url.pathComponents.reversed() {
            guard !component.hasSuffix(Constants.appSuffix) else {
                break
            }
            url.deleteLastPathComponent()
        }

        return (Bundle(url: url)?.object(forInfoDictionaryKey: kCFBundleIdentifierKey as String) as? String) ?? Constants.defaultBundleID
    }

    var sharedGroupDomain: String {
        "\(teamIDPrefix ?? Constants.defaultTeamID)\(rootBundleIdentifier)"
    }

    var teamIDPrefix: String? {
        object(forInfoDictionaryKey: Constants.teamIDPrefix) as? String
    }
}

private extension Bundle {
    var isAppExtensionBundle: Bool {
    #if APP_EXTENSION
        return true
    #else
        return false
    #endif
    }
}

private struct Constants {
    static let teamIDPrefix = "TeamIDPrefix"
    static let appSuffix = ".app"
    static let defaultTeamID = "PZYM8XX95Q"

    static let defaultBundleID: String  = {
        #if DEBUG
        "com.automattic.SimplenoteMac.Development"
        #else
        "com.automattic.SimplenoteMac"
        #endif
    }()
}
